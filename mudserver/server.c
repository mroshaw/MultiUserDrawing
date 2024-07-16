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
#include "serverconfig.h"

/* Initialise server	*/
/* Gets server details and displays welcome message */
ServerConfig * init_server(const char *config_file_name) {
    /* Get details of server from shcrc file	*/
    ServerConfig *server_config = get_server_config(config_file_name);
    fprintf(stdout, "\nServer Name: %s\nServer Port: %s", server_config->server_name, server_config->server_port);
    fprintf(stdout, "\nMaximum connections: %i", server_config->max_users);
    return server_config;
}

/* Get details from MUD Server prefs file */
/* MUST be present	*/
ServerConfig *get_server_config(const char *config_file_name) {
    FILE *config_file;
    config_file = fopen(config_file_name, "r");
    if (config_file == NULL) {
        fprintf(stderr, "Can't find prefs file: %s! Aborting!\n", config_file_name);
        /* Can't go on	*/
        exit(0);
    }

    char temp[20];
    char server_port[10];
    char server_name[30];
    int max_users;
    char file_name[30];
    char server_ip[30];
    int admin_client_id;

    fscanf(config_file, "%s %s", temp, server_port);
    fscanf(config_file, "%s %s", temp, server_name);
    fscanf(config_file, "%s %i", temp, &max_users);
    fscanf(config_file, "%s %s", temp, file_name);
    fscanf(config_file, "%s %s", temp, server_ip);
    fscanf(config_file, "%s %i", temp, &admin_client_id);

    ServerConfig *server_config = new_server_config(server_port, server_name, max_users, file_name, server_ip, admin_client_id);

    fclose(config_file);

    return server_config;
}
