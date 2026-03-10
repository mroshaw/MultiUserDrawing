/* unixserver.c                         */
/* Network code for running the server  */
/*                                      */
/* Iain Ollerenshaw 3/12/97             */
/*                                      */
/* Version History:                     */
/* Date     Who     Comments            */
/* 3/12/97  MND     Created             */
/* 6/12/97  IO      Altered for MUD     */
/* 10/3/26  IO      Added threading     */
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
#include <pthread.h>
#include "unixserver.h"
#include "parse.h"
#include "server.h"
#include "serverconfig.h"
#include "../common/utils.h"

/* Global variable to hold socket descriptor so we can      */
/* shut it down nice and tidily at the end.                 */
static int our_socket = 0;

/* Mutex to protect parse_script from concurrent access     */
static pthread_mutex_t parse_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Structure to pass connection data to each thread         */
typedef struct {
    int socket;
    ServerConfig *server_config;
} ClientConnection;

void *handle_client(void *arg)
{
    ClientConnection *conn = (ClientConnection *)arg;
    int reclen = 0;
    char buffer[BUF_SIZE];
    char result[BUF_SIZE];

    fprintf(stdout, "Client connected on socket %d.\n", conn->socket);
    fflush(stdout);

    /* Loop handling requests until client disconnects */
    while (1)
    {
        reclen = read(conn->socket, buffer, BUF_SIZE - 2);

        if (reclen <= 0)
        {
            /* Client disconnected or error */
            if (reclen == 0)
                fprintf(stdout, "Client disconnected on socket %d.\n", conn->socket);
            else
                fprintf(stderr, "Read error on socket %d (Error %d).\n", conn->socket, errno);
            fflush(stdout);
            break;
        }

        buffer[reclen] = '\0';

        fprintf(stdout, "Processing: %s\n", buffer);
        fflush(stdout);

        pthread_mutex_lock(&parse_mutex);
        parse_script(buffer, result, conn->server_config);
        pthread_mutex_unlock(&parse_mutex);

        if (write(conn->socket, result, strlen(result) + 1) < 0)
        {
            fprintf(stderr, "Write error on socket %d (Error %d).\n", conn->socket, errno);
            fflush(stderr);
            break;
        }

        fprintf(stdout, "Reply: %s\n", result);
        fflush(stdout);
    }

    /* Clean up */
    close(conn->socket);
    free(conn);
    return NULL;
}

/* Do the server loop */
void run_server(ServerConfig *server_config)
{
    int port;
    int their_namelen;
    int opt = 1;
    int them;
    struct sockaddr_in our_name;
    struct sockaddr_in their_name;

    fprintf(stdout, "Creating socket...\n");
    fflush(stdout);

    /* Create a socket to which clients can connect. */
    if ((our_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Failed to create socket (Error %d).\n", errno);
        exit(1);
    }

    /* Allow socket reuse to avoid "address already in use" on restart */
    if (setsockopt(our_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        fprintf(stderr, "Failed to set SO_REUSEADDR (Error %d).\n", errno);
        exit(1);
    }

    fprintf(stdout, "Socket successfully created.\n");
    fflush(stdout);

    port = string_to_int(server_config->server_port);

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
        signal(SIGINT,  interrupt);     /* Trap Interrupt (Ctrl-C)      */

    if (signal(SIGSEGV, SIG_IGN) != SIG_IGN)
        signal(SIGSEGV, segmentation);  /* Trap Segmentation violation  */

    if (signal(SIGBUS,  SIG_IGN) != SIG_IGN)
        signal(SIGBUS,  bus);           /* Trap Bus error               */

    if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
        signal(SIGTERM, terminate);     /* Trap Terminate               */

    listen(our_socket, 5);

    fprintf(stdout, "Server: Running! (threaded)\n");
    fflush(stdout);

    while (1)
    {
        their_namelen = sizeof(their_name);

        if ((them = accept(our_socket, (struct sockaddr *)&their_name, &their_namelen)) >= 0)
        {
            /* Allocate a connection struct to pass to the thread */
            ClientConnection *conn = malloc(sizeof(ClientConnection));
            if (conn == NULL)
            {
                fprintf(stderr, "Failed to allocate connection struct.\n");
                fflush(stderr);
                close(them);
                continue;
            }
            conn->socket        = them;
            conn->server_config = server_config;

            /* Spawn a thread to handle this connection */
            pthread_t thread;
            if (pthread_create(&thread, NULL, handle_client, conn) != 0)
            {
                fprintf(stderr, "Failed to create thread (Error %d).\n", errno);
                fflush(stderr);
                close(them);
                free(conn);
                continue;
            }

            /* Detach the thread so it cleans up automatically when done */
            pthread_detach(thread);
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
    pthread_mutex_destroy(&parse_mutex);
    close(our_socket);
    exit(value);
}