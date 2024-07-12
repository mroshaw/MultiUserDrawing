//
// Created by mrosh on 12/07/2024.
//

#include "serverdetails.h"
//
// Created by mrosh on 12/07/2024.
//
#include "serverdetails.h"

#include <stdlib.h>
#include <string.h>

ServerDetails *newServerDetails(int port, char *serverName, int maxUsers, char *fileName, char *ip, int adminId) {
    ServerDetails *newDetails = malloc(sizeof(struct ServerDetails));

    /* Port */
    newDetails->port = port;

    /* Server name */
    newDetails->serverName = malloc(strlen(serverName) + 1);
    strcpy(newDetails->serverName, serverName);

    /* Max users */
    newDetails->maxUsers = maxUsers;

    /* File name */
    newDetails->fileName = malloc(strlen(fileName) + 1);
    strcpy(newDetails->fileName, fileName);

    /* IP address */
    newDetails->ip = malloc(strlen(ip) + 1);
    strcpy(newDetails->ip, ip);

    /* Admin ID */
    newDetails->admId = adminId;

    return newDetails;
}
