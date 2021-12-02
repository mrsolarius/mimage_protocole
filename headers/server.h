#ifndef SERVER_H
#define SERVER_H

void handlerChildDeath();
void serviceProcess(int serviceSockfd);
void serverTCP(int port);
#endif // SERVER_H