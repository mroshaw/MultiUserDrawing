/* unixserver.c	                        */
/* Network code for running the server	*/
/*	                                    */
/* Iain Ollerenshaw 3/12/97	            */
/*	                                    */
/* Version History:	                    */
/* Date     Who     Comments        	*/
/* 3/12/97  MND     Created             */
/* 6/12/97  IO	    Altered for MUD	    */
/*	                                    */

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "unixserver.h"
#include "parse.h"

/* Global variable to hold socket descriptor in so we can */
/* shut it down nice and tidily at the end. Helps to reduce */
/* the number of errno=-48 errors after server crash maybe).*/
static int our_socket = 0;

/* Hold them globally so we can perfrom server functions	*/
/* outside of runServer	*/
int them;

/* Do the server loop	*/
void runServer()
{
    int port;
    int their_namelen;
    int reclen;
    int them;
    struct sockaddr_in our_name;
    struct sockaddr_in their_name;
    char buffer[BUF_SIZE];
    char result[BUF_SIZE];
    /* Create a socket to which clients can connect. */
    if ((our_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        (void)fprintf(stderr, "Failed to create socket (Error %d).\n",errno);
        (void)exit(1);
    }

    /* Initialise the socket data structure - we use a port */ /* number which is known to the client and we don't care */
    /* what machine we are running on (INADDR_ANY).	*/
    port = PORT;
    our_name.sin_family	= AF_INET;
    our_name.sin_port	= htons(port);
    our_name.sin_zero[0]	= 0;
    our_name.sin_addr.s_addr = INADDR_ANY;

    /* Assign a name to our socket ... */
    if (bind(our_socket,(struct sockaddr *)&our_name,sizeof(struct sockaddr_in)))
    {
        (void)fprintf(stderr,"Error: Failed to bind socket (Error = %d).\n",errno);
        exit(1);
    }
    /* Tell the caller what has happened. */
    fprintf(stderr,"Socket successfully created and bound.\n");

    /* Trap some of the more interesting signals ... */
    if (signal(SIGINT,SIG_IGN) != SIG_IGN)
        (void)signal(SIGINT,interrupt);

    /* Trap Interrupt (Ctrl-C) */
    if (signal(SIGSEGV,SIG_IGN) != SIG_IGN)
        signal(SIGSEGV, segmentation);

    /* Trap Segmentation violation */
    if (signal(SIGBUS,SIG_IGN) != SIG_IGN)
        (void)signal(SIGBUS,bus);

    /* Trap Bus error */
    if (signal(SIGTERM,SIG_IGN) != SIG_IGN)
        (void)signal(SIGTERM,terminate);

    /* Trap Terminate */
    /* Loop waiting for connections. If there are more than 10 */
    /* connections which are waiting to be processed then any */
    /* new client will get ECONNREFUSED when they connect().	*/
    listen(our_socket,5);
    fprintf(stderr,"Server: Running!\n");
    (void)fflush(stderr);
    while (1)
    {
        /* Got a connection - try and accept it. */
        if ((them = accept(our_socket,(struct sockaddr *)&their_name,&their_namelen))>= 0)
        {
            /* Excellent - we now have a file descriptor which */
            /* we can read/write/send to.	*/
            /* Now read the message sent to the server	*/
            /* Loop reading all the data from the socket. */
            reclen = 1;
            while (reclen <= 1)
            {
                reclen = read(them,buffer,BUF_SIZE-2);
            }
            /* We now pass the string to the parser to be dealt with */
            /* Result of parse is stored in result	*/
            fprintf(stdout,"Processing: %s\n",buffer);
            parseScript (buffer, result);
            write(them, result, strlen(result)+1);
            fprintf(stdout,"Reply: %s\n", result);
            /* Close the socket as we have finished with them. */
            // close(them);
        }
        else
        {
            fprintf(stderr,"Failed! (%d)\n",errno);
        }
    }
    /* Shutdown nice and cleanly. */
    die(0);
}

void failed(char *msg)
{
    fprintf(stderr,"%s: %s\n",SERVER_NAME,msg);
    die(1);
}

void interrupt()
{
    failed("Interrupt signal caught");
}

void segmentation ()
{
    failed("Segmentation fault caught");
}

void bus()
{
    failed("Bus error caught");
}

void terminate()
{
    failed("Terminate signal caught");
}

/* Clean up nicely	*/
void die(int value)
{
    (void)fprintf(stderr,"Server %s shutting down.\n", SERVER_NAME);
    (void) close(our_socket);

    exit(value);
}
