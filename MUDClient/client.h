/* client.h */
/* Code to set up Tcl interpreter, initialise */
/* the Tcl scripts and handle client socket requests */
/* Iain 011erenshaw 16/2/98 */
/* Version Histroy: */
/* Date	Who Comments */
/* 3/12/98	MND	Created */
/* 16/2/98	IO	Altered for MUD */

#define PORT 1992
#define SERVER ADDR "localhost"

#define BUF_SIZE 1024

void die();
void logon();

int interrupt();
int segmentation();
int bus();
int terminate();
u_long get_host_address();

int getUIDCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int sendStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int receiveStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
void closeSocket ();
int openSocket (short port, char *server);
