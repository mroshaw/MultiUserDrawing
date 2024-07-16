//
// Created by mrosh on 11/07/2024.
//

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
typedef struct ServerConfig
{
    char  *server_port;       /* Listening server_port for this server */
    char *server_name; /* Server name */
    int	max_users;	/* Maximum number of clients */
    char *file_name;	/* File serviced by server	*/
    char *server_ip;	/* IP address of server	*/
    int	admin_user_id;	/* Admin Id	*/
} ServerConfig;

ServerConfig *new_server_config(char *port, char *server_name, int max_users, char *file_name, char *ip, int admin_id);
#endif //SERVERCONFIG_H
