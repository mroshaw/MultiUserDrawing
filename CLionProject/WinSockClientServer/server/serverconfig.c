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

ServerConfig *new_server_config(char* port, char *serverName, int maxUsers, char *fileName, char *ip, int adminId) {
    ServerConfig *newServerConfig = malloc(sizeof(struct ServerConfig));

    /* Port */
    newServerConfig->port = malloc(strlen(port) + 1);
    strcpy(newServerConfig->port, port);

    /* Server name */
    newServerConfig->serverName = malloc(strlen(serverName) + 1);
    strcpy(newServerConfig->serverName, serverName);

    /* Max users */
    newServerConfig->maxUsers = maxUsers;

    /* File name */
    newServerConfig->fileName = malloc(strlen(fileName) + 1);
    strcpy(newServerConfig->fileName, fileName);

    /* IP address */
    newServerConfig->ip = malloc(strlen(ip) + 1);
    strcpy(newServerConfig->ip, ip);

    /* Admin ID */
    newServerConfig->admId = adminId;

    return newServerConfig;
}
