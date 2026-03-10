/* winclient.c                                                      */
/* Windows Winsock2 implementation of the client network layer      */
/* Replaces unixclient.c for native Windows builds                  */
/*                                                                  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tcl.h>
#include "winclient.h"
#include "common/utils.h"

/* Hold our socket globally for ease */
static SOCKET our_socket = INVALID_SOCKET;

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

    fprintf(stdout, "Attempting to connect to server %s on port %d\n", server, port);

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
    if (connect(our_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Cannot connect to server: %d\n", WSAGetLastError());
        closesocket(our_socket);
        WSACleanup();
        return -1;
    }

    fprintf(stdout, "Connected to server %s on port %d\n", server, port);

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
}

int send_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    int thePort;
    if (argc != 4) {
        fprintf(stderr, "Wrong number of args to send: %i\n", argc);
        return TCL_ERROR;
    }
    thePort = atoi(argv[2]);
    if (open_socket(thePort, argv[3]) == 0) {
        send(our_socket, argv[1], BUF_SIZE, 0);
        return TCL_OK;
    } else {
        fprintf(stderr, "winclient (send): Cannot connect to server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
}

int process_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    int thePort;
    int reclen = 0;
    static char theResult[BUF_SIZE];

    if (argc != 4) {
        fprintf(stderr, "Wrong number of args to send: %i\n", argc);
        return TCL_ERROR;
    }
    thePort = atoi(argv[2]);
    if (open_socket(thePort, argv[3]) != 0) {
        fprintf(stderr, "winclient (process): Cannot connect to server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }

    /* Send */
    send(our_socket, argv[1], BUF_SIZE, 0);

    /* Receive response */
    while (reclen == 0)
        reclen = recv(our_socket, theResult, BUF_SIZE - 2, 0);

    if (reclen < 0) {
        fprintf(stderr, "winclient (process): Failed to receive response.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }

    theResult[reclen] = '\0';
    Tcl_SetResult(interp, theResult, TCL_STATIC);
    close_socket();
    return TCL_OK;
}

int receive_string_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    int reclen = 0;
    static char theResult[BUF_SIZE];

    reclen = recv(our_socket, theResult, BUF_SIZE - 2, 0);
    if (reclen <= 0) {
        fprintf(stderr, "winclient (recv): Cannot receive from server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    theResult[reclen] = '\0';
    Tcl_SetResult(interp, theResult, TCL_STATIC);
    close_socket();
    return TCL_OK;
}

int open_socket_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    int thePort;
    if (argc != 3) {
        fprintf(stderr, "Wrong number of args to openClient: %i\n", argc);
        return TCL_ERROR;
    }
    thePort = atoi(argv[1]);
    if (open_socket(thePort, argv[2]) != 0) {
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    return TCL_OK;
}

int close_socket_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    close_socket();
    return TCL_OK;
}