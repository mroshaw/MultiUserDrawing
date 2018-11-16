/* server.c	*/
/* Routines for initialising the server.	*/
/* Does not contain network code, this can be */
/* found in unixServer.c	*/
/*	*/
/* lain 011erenshaw 13/10/97	*/
/* Version History:	*/
/* Date	Who Comments	*/
/* 13/10/97	IO	Created	*/
/*	*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "muderror.h"
/* Use global server preferences	*/
extern SDet details;
/* Initialise server	*/
/* Gets server details and displays welcome message */
void initServer()
{
    /* Get details of server from shcrc file	*/
    getDetails(details);
    fprintf(stdout, "\nServer running\nMaximum connections: %i\n", details->maxUsers);
    fprintf(stdout, "Welcome to MUDScript!\n");
}

/* Get details from MUD Server prefs file */
/* MUST be present	*/
void getDetails ()
{

    FILE *fp;
    char temp[20];
    fp=fopen("MUDShrc.cfg", "r");
    if (fp==NULL)
    {
        sendError ("getDetails: Can't find prefs file!\n Please create one!\n");
    /* Can't go on''''	*/
    exit(0);
    }
    else
    {
        fscanf(fp, "%s%i", temp, &details->maxUsers);
        fscanf(fp, "%s%s", temp, details->file);
        fscanf(fp, "%s%s", temp, details->ip);
    }
    fclose (fp);
}
