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

/* Structure to hold server details	*/
struct sDet
{
    int	maxUsers;	/* Maximum number of clients */
    char file[20];	/* File serviced by server	*/
    char ip[20];	/* IP address of server	*/
    int	admId;	/* Admin Id	*/
};

typedef struct sDet *SDet;
/* Initialise server	*/
void initServer();
/* Get server details from prefs file	*/
void getDetails ();
