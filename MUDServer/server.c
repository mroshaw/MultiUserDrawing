/* server.c	                                    */
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

#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "muderror.h"
#include "serverdetails.h"

/* Initialise server	*/
/* Gets server details and displays welcome message */
ServerDetails * initServer(const char *configFileName) {
    /* Get details of server from shcrc file	*/
    ServerDetails *serverDetails = getServerConfig(configFileName);
    fprintf(stdout, "\nServer running\n");
    fprintf(stdout, "\nServer Name: %s\nServer Port: %i", serverDetails->serverName, serverDetails->port);
    fprintf(stdout, "\nMaximum connections: %i", serverDetails->maxUsers);
    fprintf(stdout, "\n\nWelcome to MUDScript!\n");

    return serverDetails;
}

/* Get details from MUD Server prefs file */
/* MUST be present	*/
ServerDetails *getServerConfig(const char *configFileName) {
    FILE *fp;
    fp = fopen(configFileName, "r");
    if (fp == NULL) {
        sendError(sprintf("getDetails: Can't find prefs file: %s! Aborting!\n", configFileName));
        /* Can't go on''''	*/
        exit(0);
    }

    char temp[20];
    int port;
    char serverName[30];
    int maxUsers;
    char fileName[30];
    char ipAddress[30];
    int adminId;

    fscanf(fp, "%s %i", temp, &port);
    fscanf(fp, "%s %s", temp, serverName);
    fscanf(fp, "%s %i", temp, &maxUsers);
    fscanf(fp, "%s %s", temp, fileName);
    fscanf(fp, "%s %s", temp, ipAddress);
    fscanf(fp, "%s %i", temp, &adminId);

    ServerDetails *serverDetails = newServerDetails(port, serverName, maxUsers, fileName, ipAddress, adminId);

    fclose(fp);

    return serverDetails;
}
