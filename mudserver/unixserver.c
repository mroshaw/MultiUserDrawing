/* unixserver.c                         */
/* Network code for running the server  */
/*                                      */
/* Iain Ollerenshaw 3/12/97             */
/*                                      */
/* Version History:                     */
/* Date     Who     Comments            */
/* 3/12/97  MND     Created             */
/* 6/12/97  IO      Altered for MUD     */
/*                                      */

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
#include "server.h"
#include "serverconfig.h"
#include "../common/utils.h"

/* Global variable to hold socket descriptor so we can      */
/* shut it down nice and tidily at the end.                 */
static int our_socket = 0;

/* Hold them globally so we can perform server functions    */
/* outside of run_server                                    */
int them;

/* Do the server loop */
void run_server(ServerConfig *server_config)
{
    int port;
    int their_namelen;
    int reclen;
    int opt = 1;
    struct sockaddr_in our_name;
    struct sockaddr_in their_name;
    char buffer[BUF_SIZE];
    char result[BUF_SIZE];

    fprintf(stdout, "Creating socket...\n");
    fflush(stdout);

    /* Create a socket to which clients can connect. */
    if ((our_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Failed to create socket (Error %d).\n", errno);
        exit(1);
    }

    // fprintf(stdout, "Socket result: %d (errno: %d)\n", our_socket, errno);
    // fflush(stdout);

    /* Allow socket reuse to avoid "address already in use" on restart */
    if (setsockopt(our_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        fprintf(stderr, "Failed to set SO_REUSEADDR (Error %d).\n", errno);
        exit(1);
    }

    fprintf(stdout, "Socket successfully created.\n");

    /* Initialise the socket data structure - we use a port     */
    /* number which is known to the client and we don't care    */
    /* what machine we are running on (INADDR_ANY).             */
    port = string_to_int(server_config->server_port);
    // port = 1992;
    fprintf(stdout, "Binding to port %d...\n", port);
    fflush(stdout);

    our_name.sin_family      = AF_INET;
    our_name.sin_port        = htons(port);
    our_name.sin_zero[0]     = 0;
    our_name.sin_addr.s_addr = INADDR_ANY;

    /* Assign a name to our socket */
    if (bind(our_socket, (struct sockaddr *)&our_name, sizeof(struct sockaddr_in)))
    {
        fprintf(stderr, "Error: Failed to bind socket (Error = %d).\n", errno);
        exit(1);
    }

    fprintf(stdout, "Socket successfully created and bound to port %d.\n", port);
    fflush(stdout);

    /* Trap some of the more interesting signals */
    if (signal(SIGINT,  SIG_IGN) != SIG_IGN)
        signal(SIGINT,  interrupt);     /* Trap Interrupt (Ctrl-C) */

    if (signal(SIGSEGV, SIG_IGN) != SIG_IGN)
        signal(SIGSEGV, segmentation);  /* Trap Segmentation violation */

    if (signal(SIGBUS,  SIG_IGN) != SIG_IGN)
        signal(SIGBUS,  bus);           /* Trap Bus error */

    if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
        signal(SIGTERM, terminate);     /* Trap Terminate */

    /* Loop waiting for connections. If there are more than 5   */
    /* connections waiting to be processed then any new client  */
    /* will get ECONNREFUSED when they connect().               */
    listen(our_socket, 5);

    fprintf(stdout, "Server: Running!\n");
    fflush(stdout);

    while (1)
    {
        /* Initialise their_namelen before passing to accept */
        their_namelen = sizeof(their_name);

        /* Got a connection - try and accept it. */
        if ((them = accept(our_socket, (struct sockaddr *)&their_name, &their_namelen)) >= 0)
        {
            /* We now have a file descriptor we can read/write to. */
            fprintf(stdout, "Client connected.\n");
            fflush(stdout);

            /* Loop reading all the data from the socket. */
            reclen = 0;
            while (reclen <= 0)
            {
                reclen = read(them, buffer, BUF_SIZE - 2);
            }
            buffer[reclen] = '\0';

            /* Pass the string to the parser */
            fprintf(stdout, "Processing: %s\n", buffer);
            fflush(stdout);

            parse_script(buffer, result, server_config);

            write(them, result, strlen(result) + 1);

            fprintf(stdout, "Reply: %s\n", result);
            fflush(stdout);

            /* Close the connection */
            close(them);
        }
        else
        {
            fprintf(stderr, "Accept failed! (Error %d)\n", errno);
            fflush(stderr);
        }
    }

    /* Shutdown cleanly */
    die(0);
}

void failed(char *msg)
{
    fprintf(stderr, "%s: %s\n", SERVER_NAME, msg);
    die(1);
}

void interrupt()
{
    failed("Interrupt signal caught");
}

void segmentation()
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

/* Clean up nicely */
void die(int value)
{
    fprintf(stderr, "Server %s shutting down.\n", SERVER_NAME);
    close(our_socket);
    exit(value);
}