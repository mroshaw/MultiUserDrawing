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
#include "../common/utils.h"
#include "scene.h"
#include "server.h"
#include "serverconfig.h"

/* The scene simply represents a doubly linked list of objects */
/* Contained globally for simplicity	*/
Scene global_scene;

/* Run the server application	*/
int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Usage: mudServ <config_file> <scene_file> [debug]\n");
        exit(0);
    }

    /* Get command line params */
    char *config_file = argv[1];
    char *scene_file = argv[2];

    /* Allocate memory for the scene pointers */
    global_scene = (Scene) safe_malloc (sizeof (struct scene));;

    /* Get server details	*/
    ServerConfig *server_config = init_server(config_file);
    server_config->file_name = scene_file;

    /* Initialise the scene */
    global_scene->start = (Object)NULL;
    global_scene->last = (Object)NULL;

    /* Load up current server file	*/
    fprintf(stdout, "\nLoading scene from %s\n",scene_file);
    load_scene (scene_file);

    /* If in debugging, run test_parse to allow input	*/
    /* from standard input other wise run server	*/
    if (argc == 4)
    {
        test_parse(server_config);
    }
    else
    {
        run_server(server_config);
    }
    /* Explicitly save_scene before quitting	*/
    save_scene (server_config->file_name);
    return(0);
}
