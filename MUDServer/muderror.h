/* muderror.c                           */
/* Error routines for MUD server        */
/*                                      */
/* Iain Ollerenshaw  18/12/97           */
/* Version History:                     */
/* Date     Who     Comments            */
/* 18/12/97 IO      Created             */
/*                                      */

/* Lets define some error messages for the server to return! */
#ifndef MUDERROR_H
#define MUDERROR_H
#define OKAY	"1"
#define CREATEERROR	"-1"
#define ENTERERROR	"-2"
#define OBJECTERROR	"-3"
#define DELETEERROR	"-4"
#define EXISTERROR	"-5"
#define LOCKEDERROR	"-6"
#define EDITERROR "-7"
#define COMMANDERROR "-8"
#define CONNECTERROR "-9"

/* Send an error message to stderr */
void sendError(char *error);

/* Is debugging on?	*/
#define DEBUG	1
#endif // MUDERROR_H
