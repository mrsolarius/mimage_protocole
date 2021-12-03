#ifndef SERVER_H
#define SERVER_H
#include "spp.h"
#define PATH_SERVER "data/serveur/"
#define PATH_SETTINGS "data/MimeTypes.txt"
void handlerChildDeath();
void serviceProcess(int serviceSockfd);
void serverTCP(int port);

void listFilesS(int sockfd);

void downloadFileS(int sockfd, PInfoTrame info);

void uploadFile(int socketfd, PInfoTrame info);
#endif // SERVER_H