/* winclient.h                                                      */
/* Windows Winsock2 client implementation                           */
/* Replaces unixclient.h for native Windows builds                  */
/*                                                                  */
/* Iain Ollerenshaw / updated for Windows                           */
/*                                                                  */
#ifndef WINCLIENT_H
#define WINCLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tcl.h>

#define PORT        1992
#define SERVER_ADDR "localhost"
#define BUF_SIZE    1024

/* Lifecycle */
void die();
char* get_uid();

// Network operations
int connect_to_server(short port, char *server);
void disconnect_from_server();

/* Tcl command handlers */
int connect_to_server_cmd     (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int disconnect_from_server_cmd    (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int get_uid_cmd         (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int send_string_cmd     (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int receive_string_cmd  (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int process_string_cmd  (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

/* Socket operations */
int  open_socket  (short port, char *server);
void close_socket ();

#endif /* WINCLIENT_H */