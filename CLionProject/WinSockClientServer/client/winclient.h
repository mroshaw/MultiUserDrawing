//
// Created by mrosh on 15/07/2024.
//

#ifndef WINCLIENT_H
#define WINCLIENT_H

int main(int argc, char **argv);
SOCKET serverConnect(char *serverIp, char *serverPort);
int serverDisconnect(SOCKET ConnectSocket) ;
int serverSendReceive(SOCKET ConnectSocket, char *sendBuf, char *recBuf);

int getLine(char *line, int max);
#endif //WINCLIENT_H
