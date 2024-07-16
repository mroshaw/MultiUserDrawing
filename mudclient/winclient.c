//
// Created by mrosh on 11/07/2024.
//
/*	client.c		                                                                        */
/*	Code to set up Tcl interpreter, initialise the Tcl scripts and handle client socket		*/
/*	requests		                                                                        */
/*                                                                                          */
/*	Iain Ollerenshaw 16/2/98	                                                        	*/
/*                                                                                          */
/*	Version History:	                                                                	*/
/*	Date	Who     Comments	                                                            */
/*	        MND	    Created	                                                                */
/*	16/2/98 IO      Altered for MUD		                                                    */
/*			                                                                                */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "winclient.h"

#include <tcl.h>

#include "../common/utils.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1992"

SOCKET global_socket = INVALID_SOCKET;

SOCKET server_connect(char *server_ip, char *server_port) {
    WSADATA wsa_data;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int winsock_result;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    winsock_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (winsock_result != 0) {
        printf("WSAStartup failed with error: %d\n", winsock_result);
        return INVALID_SOCKET;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    winsock_result = getaddrinfo(server_ip, server_port, &hints, &result);
    if (winsock_result != 0) {
        printf("getaddrinfo failed with error: %d\n", winsock_result);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        global_socket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (global_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        winsock_result = connect(global_socket, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (winsock_result == SOCKET_ERROR) {
            closesocket(global_socket);
            global_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (global_socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    return global_socket;
}

int server_disconnect(SOCKET connect_socket) {
    // shutdown the connection since no more data will be sent

    int winsock_result = shutdown(connect_socket, SD_SEND);
    if (winsock_result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(connect_socket);
    WSACleanup();

    return 0;
}

int server_send(SOCKET connect_socket, char *send_buf) {
    int winsock_result = send(connect_socket, send_buf, DEFAULT_BUFLEN, 0);
    if (winsock_result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }

    printf("\nClient sent: %s\n", send_buf);
    return 0;
}

int server_receive(SOCKET connect_socket, char *rec_buf) {
    int winsock_result = recv(connect_socket, rec_buf, DEFAULT_BUFLEN, 0);
    if (winsock_result > 0) {
        printf("Message received: %s\n", rec_buf);
    } else if (winsock_result == 0) {
        printf("Connection closed\n");
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }
    return winsock_result;
}

int server_send_receive(SOCKET connect_socket, char *send_buf, char *rec_buf) {
    int winsock_result = send(connect_socket, send_buf, DEFAULT_BUFLEN, 0);
    if (winsock_result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return winsock_result;
    }

    winsock_result = recv(connect_socket, rec_buf, DEFAULT_BUFLEN, 0);
    if (winsock_result > 0) {
        printf("Bytes received: %d\n", winsock_result);
        printf("Message received: %s\n", rec_buf);
    } else if (winsock_result == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());
    printf("Command processed.\n");

    return winsock_result;
}

int close_client_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]) {
    server_disconnect(global_socket);
    return TCL_OK;
}

/* These client commands handle, at a more abstract level, the sending */
/* and receiving of strings. They are directly linked to Tcl commands */
/* which facilitates communication between the GUI layer and the client */
/* C layer	*/
int send_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]) {
    fprintf(stdout, "In: send_string_cmd'");
    if (argc != 4) {
        fprintf(stderr, "Wrong number of args to 'send_string_cmd'! HELP!: %i\n",
                argc);
        exit(0);
    }

    int winsock_result = server_send(global_socket, argv[1]);

    if (winsock_result != 0) {
        return TCL_ERROR;
    }
    return TCL_OK;
}

int process_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]) {
    fprintf(stdout, "In: process_string_cmd'");
    char recbuf[DEFAULT_BUFLEN];
    int winsock_result = server_send(global_socket, argv[1]);
    if (winsock_result != 0) {
        return TCL_ERROR;
    }
    winsock_result = server_receive(global_socket, recbuf);
    if (winsock_result < 0) {
        return TCL_ERROR;
    }
    fprintf(stdout, "process_string_cmd successful. %s", recbuf);
    Tcl_SetResult(interp, recbuf, TCL_VOLATILE );
    return TCL_OK;
}

int receive_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]) {
    fprintf(stdout, "In: receive_string_cmd'");

    // Send an initial buffer
    char recbuf[DEFAULT_BUFLEN];
    int winsock_result = server_receive(global_socket, recbuf);
    if (winsock_result != 0) {
        return TCL_ERROR;
    }
    Tcl_SetResult(interp, recbuf, TCL_VOLATILE);
    return TCL_OK;
}

/* Get the users unique id	*/
int get_uid_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]) {
    static char client_id[20];
    /* int cId = (int)getuid(); */
    int cId = 1;
    int_to_string(cId, client_id);
    Tcl_SetResult(interp, client_id, TCL_VOLATILE);
    return TCL_OK;
}
