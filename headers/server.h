#ifndef SERVER_H
#define SERVER_H
#include "spp.h"
void handlerChildDeath();
void serviceProcess(int serviceSockfd);
void serverTCP(int port);

void listFilesS(int sockfd);

void downloadFileS(int sockfd, PInfoTrame info);

#endif // SERVER_H