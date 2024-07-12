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
#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE

#include <ctype.h>
#include <io.h>
#include <sysinfoapi.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include "utils.h"
#include "winclient.h"

/* Hold our socket globally for ease	*/
static int our_socket = 0;

u_long get_host_address(char *host) {
    /* Return the host ID given its name or IP address. */
    u_long addr;

    struct hostent *host_entry;
    if ((host == NULL) || !host[0])
        return INADDR_ANY;
    if (isdigit(host[0])) {
        /* Address in internet format (eg. 138.251.11.6) */
        if ((long) (addr = (u_long) inet_addr(host)) == -1) {
            (void) fprintf(stderr, "Could not find host\"%s\".\n", host);
            exit(1);
        }
        return addr;
    } else {
        if ((host_entry = gethostbyname(host)) == NULL) {
            (void) fprintf(stderr, "Could not find host\"%s\".\n", host);
            exit(1);
        }
        return ((u_long) ((struct in_addr *) (host_entry->h_addr))->s_addr);
    }
}

void logon(int our_socket) {
    int reclen;
    char buffer[BUF_SIZE];
    /* Loop reading all the data from the socket. */
    reclen = 1;
    while (reclen > 0) {
        reclen = recv(our_socket, buffer,BUF_SIZE - 2, 0);
    }

    if (reclen > 0) {
        (void) printf("%s", buffer);
    }
}

void failed(char *msg) {
    (void) fprintf(stderr, "%s: %s\n", "Error:", msg);
    die(1);
}

int interrupt() {
    failed("Interrupt signal caught");
    return -1;
}

int segmentation() {
    failed("Segmentation fault caught");
    return -1;
}

int bus() {
    failed("Bus error caught");
    return -1;
}

int terminate() {
    failed("Terminate signal caught");
    return -1;
}

void die(int value) {
    exit(value);
}

/* These client commands handle, at a more abstract level, the sending */
/* and receiving of strings. They are directly linked to Tcl commands */
/* which facilitates communication between the GUI layer and the client */
/* C layer	*/
int sendStringCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int thePort;
    if (argc != 4) {
        fprintf(stderr, "Wrong number of args to send! HELP!: %i\n",
                argc);
        exit(0);
    }
    thePort = atoi(argv[2]);
    if (openSocket(thePort, argv[3]) == 0) {
        send(our_socket, argv[1], BUF_SIZE, 0);
        return TCL_OK;
    } else {
        fprintf(stderr, "mudClient (send): Cannot connect to the server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
}

int processStringCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int thePort;
    int reclen = 0;
    static char theString[BUF_SIZE];
    static char theResult[BUF_SIZE];

    // First, send
    if (argc != 4) {
        fprintf(stderr, "Wrong number of args to send! HELP!: %i\n",
                argc);
        exit(0);
    }
    // Open soecket
    thePort = atoi(argv[2]);
    if (openSocket(thePort, argv[3]) == 0) {
        // Send
        // send (our_socket, argv[1], BUF_SIZE, 0);
        write(our_socket, argv[1], BUF_SIZE);
    } else {
        fprintf(stderr, "mudClient (sendRevc - send): Cannot connect to the server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }

    // Artificial delay
    int wait = 1;
    while (wait > 0) {
        wait--;
    }
    // Receive response
    while (reclen == 0) {
        reclen = read(our_socket, theResult,BUF_SIZE - 2);
    }
    if (reclen < 0) {
        /* Error. Server not responding if (reclen <= 0) */
        fprintf(stderr, "mudClient (sendRevc - recv): Failed to receive response from server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    /* String received okay */
    Tcl_SetResult(interp, theResult, TCL_STATIC);
    // interp->result = theResult;

    /* Good manners! */
    closeSocket();
    return TCL_OK;
}

int receiveStringCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    int reclen = 0;
    static char theString[BUF_SIZE];
    reclen = recv(our_socket, theString,BUF_SIZE - 2, 0);
    if (reclen <= 0) {
        /* Error. Server not responding if (reclen <= 0) */
        fprintf(stderr, "mudClient (recv): Cannot connect to the server.\n");
        Tcl_SetResult(interp, "-9", TCL_STATIC);
        return TCL_ERROR;
    }
    /* String received okay */
    Tcl_SetResult(interp, theString, TCL_STATIC);

    /* Good manners! */
    closeSocket();
    return TCL_OK;
}

/* Open the socket connection */
int openSocket(short port, char *server) {
    /* Define the socket structure */
    struct sockaddr_in our_name;

    /* Init Win sockets */
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    /* Create a socket which we can connect to. */
    if ((our_socket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Failed to create socket (Error = %-d).\n",errno);
        return -1;
    }

    /* Initialise the socket information */
    our_name.sin_family = AF_INET;
    our_name.sin_port = htons(port);
    our_name.sin_zero[0] = 0;
    our_name.sin_addr.s_addr = get_host_address(server);

    /* Connect to the named socket. */
    if (connect(our_socket, (struct sockaddr *) &our_name, sizeof(struct sockaddr_in))) {
        fprintf(stderr, "mudClient (open): Cannot connect to the server.\n");
        return -1;
    }
    return 0;
}

/* Close the socket connection	*/
void closeSocket() {
    /* Shutdown the socket (good manners) */
    (void) shutdown(our_socket, 2);
    exit(0);
}

/* Get the users unique id	*/
int getUIDCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
    static char theId[20];
    /* int cId = (int)getuid(); */
    int cId = 1;
    my_itoa(cId, theId);
    Tcl_SetResult(interp, theId, TCL_STATIC);
    return TCL_OK;
}
