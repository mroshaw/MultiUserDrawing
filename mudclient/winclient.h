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
#ifndef WINCLIENT_H
#define WINCLIENT_H

#define PORT 1992
#define SERVER_ADDR "localhost"
#define BUF_SIZE 1024
#include <tcl.h>
#include <winsock2.h>

SOCKET server_connect(char *server_ip, char *server_port);

int server_disconnect();

int server_send(SOCKET connect_socket, char *send_buf);

int server_receive(SOCKET connect_socket, char *rec_buf);

int server_send_receive(SOCKET connect_socket, char *send_buf, char *rec_buf);

int close_client_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]);

int get_uid_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]);

int send_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]);

int receive_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]);

int process_string_cmd(ClientData client_data, Tcl_Interp *interp, int argc, char *argv[]);

#endif // WINCLIENT_H
