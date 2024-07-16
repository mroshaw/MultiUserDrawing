#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include "winserver.h"

#include <stdbool.h>

#include "parse.h"
#include "serverconfig.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512

int them;

int run_server(ServerConfig *server_config) {
    WSADATA wsa_data;
    int winsock_result;

    SOCKET listen_socket = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    winsock_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (winsock_result != 0) {
        printf("WSAStartup failed with error: %d\n", winsock_result);
        return winsock_result;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and server_port
    winsock_result = getaddrinfo(NULL, server_config->server_port, &hints, &result);
    if (winsock_result != 0) {
        printf("getaddrinfo failed with error: %d\n", winsock_result);
        WSACleanup();
        return winsock_result;
    }

    // Create a SOCKET for the server to listen for client connections.
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return winsock_result;
    }

    // Setup the TCP listening socket
    winsock_result = bind(listen_socket, result->ai_addr, (int) result->ai_addrlen);
    if (winsock_result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        cleanup_server(listen_socket);
        return winsock_result;
    }

    freeaddrinfo(result);

    printf("Server listening on port: %s\n", server_config->server_port);

    while (true) {
        winsock_result = listen(listen_socket, SOMAXCONN);
        printf("Waiting for client connection...\n");
        if (winsock_result == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            cleanup_server(listen_socket);
            return winsock_result;
        }

        // Accept a client socket
        client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            cleanup_server(listen_socket);
            return winsock_result;
        }

        printf("Client connected. Dispatching thread.\n");

        // Issue a thread to manage this client
        ThreadParams thread_params;
        thread_params.client_socket = client_socket;
        thread_params.server_config = server_config;

        unsigned thread_id;
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &client_session, (void*)&thread_params, 0, &thread_id);
        printf("Thread dispatched: %i\n", thread_id);
    }
}

unsigned __stdcall client_session(void *params_data) {
    // Receive until the peer shuts down the connection

    ThreadParams *thread_params = (ThreadParams*)params_data;
    SOCKET client_socket = thread_params->client_socket;
    ServerConfig *server_config = thread_params->server_config;
    int winsock_result;
    char rec_buf[DEFAULT_BUFLEN];

    do {
        char parsereplybuf[DEFAULT_BUFLEN];

        winsock_result = recv(client_socket, rec_buf, DEFAULT_BUFLEN, 0);
        if (winsock_result > 0) {
            printf("Message received: %s\n", rec_buf);

            parse_script(rec_buf, parsereplybuf, server_config);

            printf("Parse result: %s\n", parsereplybuf);

            winsock_result = send(client_socket, parsereplybuf, DEFAULT_BUFLEN, 0);
            if (winsock_result == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                cleanup_client(client_socket);
            }
            // printf("Bytes sent: %d\n", iSendResult);
            printf("Response sent: %s\n", parsereplybuf);
        } else if (winsock_result == 0) {
            printf("Client disconnected.\n");
            return 0;
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            cleanup_client(client_socket);
        }
    } while (winsock_result > 0);
}

void cleanup_client(SOCKET client_socket) {
    closesocket(client_socket);
}

void cleanup_server(SOCKET server_socket) {
    closesocket(server_socket);
    WSACleanup();
}
