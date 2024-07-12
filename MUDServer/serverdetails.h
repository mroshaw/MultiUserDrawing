//
// Created by mrosh on 11/07/2024.
//

#ifndef SERVERDETAILS_H
#define SERVERDETAILS_H
typedef struct ServerDetails
{
    int port;       /* Listening port for this server */
    char *serverName; /* Server name */
    int	maxUsers;	/* Maximum number of clients */
    char *fileName;	/* File serviced by server	*/
    char *ip;	/* IP address of server	*/
    int	admId;	/* Admin Id	*/
} ServerDetails;

ServerDetails *newServerDetails(int port, char *serverName, int maxUsers, char *fileName, char *ip, int adminId);
#endif //SERVERDETAILS_H
