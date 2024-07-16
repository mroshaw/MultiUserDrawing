//
// Created by mrosh on 11/07/2024.
//

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
typedef struct ServerConfig
{
    char  *port;       /* Listening port for this server */
    char *serverName; /* Server name */
    int	maxUsers;	/* Maximum number of clients */
    char *fileName;	/* File serviced by server	*/
    char *ip;	/* IP address of server	*/
    int	admId;	/* Admin Id	*/
} ServerConfig;

ServerConfig *new_server_config(char *port, char *serverName, int maxUsers, char *fileName, char *ip, int adminId);
#endif //SERVERCONFIG_H
