/* unixserver.h	*/
/* Network code for running the server	*/
/*	*/
/* Lain 011erenshaw 3/12/97	*/
/* Version History:	*/
/* Date	Who Comments	*/
/* 3/12/97 MND Created	*/

/* 6/12/97 IO	Altered for MUD	*/
/*	*/
/* These defines determine the same if the server, the string buffer */
/* size and the port number to use	*/
#define BUF_SIZE 1024
#define SERVER_NAME	"MUDServer"
#define PORT	1992

/* Routines specific to this server. */
void runServer(void);
void die();
void welcome();

/* Detect errors */
void interrupt();
void segmentation();
void bus();
void terminate();
u_long get_host_address();
/* Useful system routines. */
FILE *input();
