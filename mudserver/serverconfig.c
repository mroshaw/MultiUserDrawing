//
// Created by mrosh on 12/07/2024.
//

#include "serverconfig.h"
//
// Created by mrosh on 12/07/2024.
//
#include "serverconfig.h"

#include <stdlib.h>
#include <string.h>

ServerConfig *new_server_config(char* port, char *server_name, int max_users, char *file_name, char *ip, int admin_id) {
    ServerConfig *new_server_config = malloc(sizeof(struct ServerConfig));

    /* Port */
    new_server_config->server_port = malloc(strlen(port) + 1);
    strcpy(new_server_config->server_port, port);

    /* Server name */
    new_server_config->server_name = malloc(strlen(server_name) + 1);
    strcpy(new_server_config->server_name, server_name);

    /* Max users */
    new_server_config->max_users = max_users;

    /* File name */
    new_server_config->file_name = malloc(strlen(file_name) + 1);
    strcpy(new_server_config->file_name, file_name);

    /* IP address */
    new_server_config->server_ip = malloc(strlen(ip) + 1);
    strcpy(new_server_config->server_ip, ip);

    /* Admin ID */
    new_server_config->admin_user_id = admin_id;

    return new_server_config;
}
