/* unixserver.h	                        */
/* Network code for running the server	*/
/*	                                    */
/* Iain Ollerenshaw 3/12/97	            */
/*	                                    */
/* Version History:	                    */
/* Date     Who     Comments        	*/
/* 3/12/97  MND     Created             */
/* 6/12/97  IO	    Altered for MUD	    */
/*	                                    */

#include "serverconfig.h"

#define BUF_SIZE        1024
#define SERVER_NAME     "MUDServer"
#define PORT            1992

/* Routines specific to this server. */
void run_server(ServerConfig *server_config);
void die();
void welcome();

/* Detect errors */
void interrupt();
void segmentation();
void bus();
void terminate();
unsigned long get_host_address();

/* Useful system routines. */
FILE *input();