#ifndef SERVER_H
#define SERVER_H

void handlerChildDeath();
void serviceProcess(int serviceSockfd);
void serverTCP(int port);

void listFilesS(int sockfd);

#endif // SERVER_H