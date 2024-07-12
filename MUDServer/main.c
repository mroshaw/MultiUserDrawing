/* main.c	                                */
/* Main run routines. Calls init and runs   */
/* server	                                */
/*	                                        */
/* lain Ollerenshaw 13/10/97	            */
/*	                                        */
/* Version History:	                        */
/* Date     Who     Comments            	*/
/* 13/10/97	IO	    Created	                */
/*	                                        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "parse.h"
#ifdef __WIN32__
#include "winserver.h"
#else
#include "unixserver.h"
#endif
#include "file.h"
#include "utils.h"
#include "scene.h"
#include "server.h"
#include "serverdetails.h"

/* The scene simply represents a doubly linked list of objects */
/* Contained globally for simplicity	*/
Scene theScene;

/* Run the server application	*/
int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Usage: mudServ <config_file> <scene_file> [debug]\n");
        exit(0);
    }

    /* Get command line params */
    char *configfile = argv[1];
    char *scenefile = argv[2];

    /* Allocate memory for the scene pointers */
    theScene = (Scene) safemalloc (sizeof (struct scene));;

    /* Get server details	*/
    ServerDetails *serverDetails = initServer(configfile);

    /* Initialise the scene */
    theScene->start = (Object)NULL;
    theScene->last = (Object)NULL;

    /* Load up current server file	*/
    fprintf(stdout, "\nLoading scene from %s\n",scenefile);
    load (scenefile);

    /* If in debugging, run testParse to allow input	*/
    /* from standard input other wise run server	*/
    if (argc == 4)
    {
        testParse(serverDetails);
    }
    else
    {
        runServer(serverDetails);
    }
    /* Explicitly save before quitting	*/
    save (serverDetails->fileName);
    return(0);
}
