/*	client.c		*/
/*	Code to set up Tcl interpreter, initialise the Tcl scripts and handle client socket		*/
/*			*/
/*	requests		*/
/*	lain 011erenshaw 16/2/98		*/
/*	Version History:		*/
/*	Date	Who	Comments	*/
/*	MND	Created	*/
/*	16/2/98	10	Altered for MUD		*/
/*			*/
#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE

#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <tcl.h>
#include <unistd.h>
#include "tclInit.h"
#include "client.h"
#include "utils.h"

/* Hold our socket globally for ease	*/
static int our_socket = 0;

u_long get_host_address(host)
char *host;
{
	/* Return the host ID given its name or IP address. */
	u_long addr;


	struct hostent *host_entry;
	if ((host == NULL) || !host[0])
		return INADDR_ANY;
	if (isdigit(host[0]))
	{
		/* Address in internet format (eg. 138.251.11.6) */
		if ((long)(addr = (u_long)inet_addr(host)) == -1)
		{
			(void)fprintf(stderr,"Could not find host\"%s\".\n",host);
			exit(1);
	}
		return addr;
	}
	else
	{
		if ((host_entry = gethostbyname(host)) == NULL)
		{
			(void)fprintf(stderr,"Could not find host\"%s\".\n",host);
			exit(1);
		}
		return ((u_long)((struct in_addr *)(host_entry -> h_addr)) -> s_addr);
	}
}

void logon(int our_socket)
{
	int reclen;
	char buffer[BUF_SIZE];
	/* Loop reading all the data from the socket. */ reclen = 1;
	while (reclen > 0)
        reclen = recv(our_socket,buffer,BUF_SIZE - 2,0);

	if (reclen > 0)
	(void)printf("%s",buffer);
}

void failed(char *msg)
{
	(void)fprintf(stderr,"%s: %s\n","Error:",msg);
	die(1);
}

int interrupt()
{
	failed("Interrupt signal caught");
	return -1;
}

int segmentation()
{
	failed("Segmentation fault caught");
	return -1;
}

int bus()
{
	failed("Bus error caught");
	return -1;
}

int terminate()
{
	failed("Terminate signal caught");
	return -1;
}

void die(int value)
{
	exit(value);
}

/* These client commands handle, at a more abstract level, the sending */
/* and receiving of strings. They are directly linked to Tcl commands */
/* which facilitates communication between the GUI layer and the client */
/* C layer	*/
int sendStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	int thePort;
	if (argc != 4) {
		fprintf(stderr, "Wrong number of args to send! HELP!: %i\n",
		argc);
		exit(0);
	}
	thePort = atoi (argv[2]);
	if (openSocket(thePort, argv[3]) == 0)
    {
        send (our_socket, argv[1], BUF_SIZE, 0);
        return TCL_OK;
    }
    else
    {
        fprintf(stderr, "mudClient (send): Cannot connect to the server.\n");
		interp->result = "-9";
        return TCL_ERROR;
    }
}

int receiveStringCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	int reclen = 0;
	static char theString[BUF_SIZE];
	reclen = recv(our_socket,theString,BUF_SIZE - 2,0);
	if (reclen <= 0)
	{
		/* Error. Server not responding if (reclen <= 0) */
		fprintf(stderr, "mudClient (recv): Cannot connect to the server.\n");
		interp->result = "-9";
		return TCL_ERROR;
	}
	/* String received okay */
	interp->result=theString;

	/* Good manners! */
	closeSocket ();
	return TCL_OK;
}

/* Open the socket connection */
int openSocket (short port, char *server)
{
	/* struct sockaddr_in our_name; */
	struct sockaddr_in our_name;
	char sendBuffer[BUF_SIZE];
	char recBuffer[BUF_SIZE];
	int reclen;
	/* Create a socket which we can connect to. */
	if ((our_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		fprintf(stderr,"Failed to create socket (Error = %-d).\n",errno);
		return -1;
	}

	/* Initialise the socket information */
	our_name.sin_family	= AF_INET;
	our_name.sin_port	= htons(port);
	our_name.sin_zero[0]	= 0;
	our_name.sin_addr.s_addr = get_host_address(server);

	/* Connect to the named socket. */
	if (connect(our_socket,&our_name,sizeof(struct sockaddr_in)))
	{
		fprintf(stderr,"mudClient (open): Cannot connect to the	server.\n");
		return -1;
	}
	return 0;
}

/* Close the socket connection	*/
void closeSocket()
{
	/* Shutdown the socket (good manners) */
	(void)shutdown(our_socket,2);
	/* (void)die(0); */
}

/* Get the users unique id	*/
int getUIDCmd (ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	static char theId[20];
	int cId = (int)getuid();
	/* int cId = 1234; */
	my_itoa(cId, theId);
	interp->result=theId;
	return TCL_OK;
}


