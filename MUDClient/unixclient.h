/* unixclient.h                                                                                 */
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

#define PORT 1992
#define SERVER_ADDR "localhost"
#define BUF_SIZE 1024

void die();
void logon();

int interrupt();
int segmentation();
int bus();
int terminate();
unsigned long get_host_address();

int getUIDCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int sendStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int receiveStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int processStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
void closeSocket ();
int openSocket (short port, char *server);
