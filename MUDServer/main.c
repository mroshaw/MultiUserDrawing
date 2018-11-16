/* main.c	*/
/* Main run routines. Calls init and runs */
/* server	*/
/*	*/
/* lain 011erenshaw 13/10/97	*/
/* Version Histroy:	*/
/* Date	Who Comments	*/
/* 13/10/97	IO	Created	*/
/*	*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "parse.h"
#include "unixserver.h"
#include "server.h"
#include "file.h"
#include "utils.h"
#include "muderror.h"
#include "scene.h"
/* The scene simply represents a doubly linked list of objects */
/* Contained globally for simplicity	*/
Scene theScene;

/* As are the server prefs	*/
SDet details;

/* Run the server application	*/
int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: mudSery <file>\n");
        exit(0);
    }

    /* Allocate server details	*/
    details = (SDet) safemalloc (sizeof (struct sDet));

    /* Allocate memory for the scene pointers */
    theScene = (Scene) safemalloc (sizeof (struct scene));;

    /* Get server details	*/
    initServer();

    /* Initialise the scene */
    theScene->start = (Object)NULL;
    theScene->last = (Object)NULL;

    /* Load up current server file	*/
    fprintf(stdout, "\nLoading scene from %s\n",argv[1]);
    load (argv[1]);
    strcpy(details->file, argv[1]);
    /* If in debugging, run testParse to allow input	*/
    /* from standard input other wise run server	*/
    if (argc == 3)
    {
        testParse();
    }
    else
    {
        runServer();
    }
    /* Explicitly save before quitting	*/
    save (details->file);
    return(0);
}
