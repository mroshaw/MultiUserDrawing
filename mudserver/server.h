/* server.h	                                    */
/* Routines for initialising the server.	    */
/* Does not contain network code, this can be   */
/* found in unixServer.c	                    */
/*	                                            */
/* Iain Ollerenshaw 13/10/97	                */
/*	                                            */
/* Version History:                             */
/* Date     Who     Comments                	*/
/* 13/10/97	IO  	Created	                    */
/*	                                            */

/* These defines determine the string buffer size */

#ifndef SERVER_H
#define SERVER_H

#include "serverconfig.h"
#define BUF_SIZE 1024

/* Initialise server	*/
ServerConfig *init_server(const char *config_file_name);

/* Get server details from prefs file	*/
ServerConfig *get_server_config(const char *config_file_name);
#endif //SERVER_H
