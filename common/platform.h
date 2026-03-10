/* platform.h                                       */
/* Central platform detection and compatibility     */
/* Include this in all source files                 */
/*                                                  */
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    /* Windows */
    #include <windows.h>
    #include <direct.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define PATH_SEP        "\\"
    #define getcwd          _getcwd
    #define PATH_MAX        MAX_PATH
    #include "../mudclient/winclient.h"
#else
    /* Unix / Cygwin */
    #include <unistd.h>
    #include <limits.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #define PATH_SEP        "/"
    #include "../mudclient/unixclient.h"
#endif

#endif /* PLATFORM_H */