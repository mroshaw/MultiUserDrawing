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

#include "serverdetails.h"
#define BUF_SIZE 1024

/* Initialise server	*/
ServerDetails *initServer(const char *configFileName);

/* Get server details from prefs file	*/
ServerDetails *getServerConfig(const char *configFileName);
#endif //SERVER_H
