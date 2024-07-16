/* winserver.h	                        */
/* Network code for running the server	*/
/*	                                    */
/* Iain Ollerenshaw 3/12/97	            */
/*	                                    */
/* Version History:	                    */
/* Date     Who     Comments        	*/
/* 3/12/97  MND     Created             */
/* 6/12/97  IO	    Altered for MUD	    */
/*	                                    */
/* Routines specific to this server. */

#ifndef WINSERVER_H
#define WINSERVER_H
#include "serverconfig.h"
#include <winsock2.h>

int run_server(ServerConfig *server_config);

unsigned __stdcall client_session(void *data);

void cleanup_client(SOCKET client_socket);

void cleanup_server(SOCKET server_socket);
#endif // WINSERVER_H

typedef struct threadParams {
    SOCKET client_socket;
    ServerConfig *server_config;
} ThreadParams;