#define WIN32_LEAN_AND_MEAN

#include <stdbool.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "winclient.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1992"

int main(int argc, char **argv) {
    int iResult;

    // Validate the parameters
    if (argc != 3) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    SOCKET ConnectSocket = serverConnect(argv[1], argv[2]);

    if (ConnectSocket != INVALID_SOCKET) {
        printf("Connected to server...\n\n");
    } else {
        printf("Failed to connect to server!");
        return -1;
    }

    char msgbuf[DEFAULT_BUFLEN];
    char rcvbuf[DEFAULT_BUFLEN];
    strcpy(msgbuf, "");

    while (strcmp(msgbuf, "EXIT") != 0) {
        // Get standard input
        printf("> ");
        getLine(msgbuf, DEFAULT_BUFLEN);

        // Send the message
        iResult = serverSendReceive(ConnectSocket, msgbuf, rcvbuf);

        if (iResult != 0) {
            printf("Failed to send message to server!");
            return -1;
        }
    }

    iResult = serverDisconnect(ConnectSocket);

    if (iResult != 0) {
        return -1;
    }

    return 0;
}

SOCKET serverConnect(char *serverIp, char *serverPort) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return INVALID_SOCKET;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(serverIp, serverPort, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    return ConnectSocket;
}

int serverDisconnect(SOCKET ConnectSocket) {
    // shutdown the connection since no more data will be sent

    int iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

int serverSendReceive(SOCKET ConnectSocket, char *sendBuf, char *recBuf) {
    int iResult;

    iResult = send(ConnectSocket, sendBuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("\nBytes Sent: %ld\n", iResult);

    iResult = recv(ConnectSocket, recBuf, DEFAULT_BUFLEN, 0);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
        printf("Message received: %s\n", recBuf);
    } else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());
    printf("Command processed.\n");

    return 0;
}

/* Get a line from standard input   */
int getLine(char *line, int max) {
    if (fgets(line, max, stdin) == NULL) {
        return 0;
    } else {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        return strlen(line);
    }
}
