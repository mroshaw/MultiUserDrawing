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

int main(int argc, char *argv[]);

void die();
void logon();

int interrupt();
int segmentation();
int bus();
int terminate();
unsigned long get_host_address();

int get_uid_cmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
char* get_uid();
int send_string_cmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int receive_string_cmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int process_string_cmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
void open_socket_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
void close_socket_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int open_socket (short port, char *server);
void closeSocket ();