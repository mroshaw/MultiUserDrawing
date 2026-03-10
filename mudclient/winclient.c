/* winclient.c                                                      */
/* Windows Winsock2 implementation of the client network layer      */
/* Replaces unixclient.c for native Windows builds                  */
/*                                                                  */
/* Iain Ollerenshaw 16/2/98                                         */
/*                                                                  */
/* Version History:                                                 */
/* Date         Who     Comments                                    */
/*              MND     Created                                     */
/* 16/2/98      IO      Altered for MUD                             */
/* 2024         IO      Persistent connection support               */
/*                                                                  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tcl.h>
#include "winclient.h"
#include "../common/utils.h"

/* Hold our socket globally for ease */
static SOCKET our_socket = INVALID_SOCKET;

/* Persistent connection state */
static int is_connected = 0;

/* Initialise Winsock - must be called before any socket operations */
static int init_winsock()
{
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", result);
        return -1;
    }
    return 0;
}

void die()
{
    close_socket();
    WSACleanup();
    exit(1);
}

char* get_uid()
{
    static char theId[20];
    int cId = 1;
    int_to_string(cId, theId);
    return theId;
}

int get_uid_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char *theId = get_uid();
    Tcl_SetResult(interp, theId, TCL_STATIC);
    return TCL_OK;
}

/* Open the socket connection */
int open_socket(short port, char *server)
{
    struct sockaddr_in server_addr;
    struct hostent *host_entry;

    if (init_winsock() != 0)
        return -1;

    /* Create socket */
    our_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (our_socket == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    /* Resolve host */
    host_entry = gethostbyname(server);
    if (host_entry == NULL) {
        fprintf(stderr, "Could not resolve host: %s\n", server);
        closesocket(our_socket);
        WSACleanup();
        return -1;
    }

    /* Set up address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = ((struct in_addr *)(host_entry->h_addr))->s_addr;

    /* Connect */
    fprintf(stdout, "Attempting to connect to server %s on port %d\n", server, port);
    fflush(stdout);

    if (connect(our_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Cannot connect to server: %d\n", WSAGetLastError());
        closesocket(our_socket);
        WSACleanup();
        return -1;
    }
    return 0;
}

/* Close the socket connection */
void close_socket()
{
    if (our_socket != INVALID_SOCKET) {
        shutdown(our_socket, SD_BOTH);
        closesocket(our_socket);
        our_socket = INVALID_SOCKET;
    }
    WSACleanup();
    is_connected = 0;
}

/* Connect once at startup */
int connect_to_server(short port, char *server)
{
    if (is_connected)
    {
        fprintf(stdout, "Already connected to server.\n");
        fflush(stdout);
        return 0;
    }
    if (open_socket(port, server) != 0)
    {
        fprintf(stderr, "Failed to connect to server.\n");
        fflush(stderr);
        return -1;
    }
    is_connected = 1;
    fprintf(stdout, "Connected to server %s:%d\n", server, port);
    fflush(stdout);
    return 0;
}

/* Disconnect on quit */
void disconnect_from_server()
{
    if (is_connected)
    {
        close_socket();
        is_connected = 0;
        fprintf(stdout, "Disconnected from server.\n");
    }
}


/* Send and receive on the persistent connection */
int send_receive(const char *message, char *result)
{
    int reclen = 0;

    if (!is_connected)
    {
        fprintf(stderr, "send_receive: Not connected to server.\n");
        fflush(stderr);
        return -1;
    }

    /* Send */
    if (send(our_socket, message, strlen(message) + 1, 0) == SOCKET_ERROR)
    {
        fprintf(stderr, "send_receive: Send failed: %d\n", WSAGetLastError());
        fflush(stderr);
        is_connected = 0;
        return -1;
    }

    /* Receive */
    while (reclen == 0)
        reclen = recv(our_socket, result, BUF_SIZE - 2, 0);

    if (reclen == SOCKET_ERROR)
    {
        fprintf(stderr, "send_receive: Receive failed: %d\n", WSAGetLastError());
        fflush(stderr);
        is_connected = 0;
        return -1;
    }

    result[reclen] = '\0';
    return 0;
}

/* Tcl command: sendString <message>        */
/* Send a string with no response expected  */
int send_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "sendString: wrong number of args: %i\n", argc);
        return TCL_ERROR;
    }
    if (!is_connected)
    {
        fprintf(stderr, "sendString: Not connected to server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    if (send(our_socket, argv[1], strlen(argv[1]) + 1, 0) == SOCKET_ERROR)
    {
        fprintf(stderr, "sendString: Send failed: %d\n", WSAGetLastError());
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    return TCL_OK;
}

/* Tcl command: sendReceiveString <message> */
/* Send a string and receive a response     */
int process_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static char theResult[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "sendReceiveString: wrong number of args: %i\n", argc);
        return TCL_ERROR;
    }
    if (!is_connected)
    {
        fprintf(stderr, "sendReceiveString: Not connected to server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    if (send_receive(argv[1], theResult) != 0)
    {
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_SetResult(interp, theResult, TCL_STATIC);
    return TCL_OK;
}

/* Tcl command: receiveString               */
/* Receive a string from the server         */
int receive_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static char theResult[BUF_SIZE];
    int reclen = 0;

    if (!is_connected)
    {
        fprintf(stderr, "receiveString: Not connected to server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    reclen = recv(our_socket, theResult, BUF_SIZE - 2, 0);
    if (reclen <= 0)
    {
        fprintf(stderr, "receiveString: Receive failed: %d\n", WSAGetLastError());
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    theResult[reclen] = '\0';
    Tcl_SetResult(interp, theResult, TCL_STATIC);
    return TCL_OK;
}

/* Tcl command: openClient <port> <server>  */
/* Connect to the server                    */
int connect_to_server_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    int thePort;
    if (argc != 3) {
        fprintf(stderr, "openClient: wrong number of args: %i\n", argc);
        return TCL_ERROR;
    }
    thePort = atoi(argv[1]);
    if (connect_to_server(thePort, argv[2]) != 0)
    {
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    return TCL_OK;
}

/* Tcl command: closeClient                 */
/* Disconnect from the server               */
int disconnect_from_server_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    disconnect_from_server();
    return TCL_OK;
}