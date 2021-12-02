#ifndef CLIENT_H
#define CLIENT_H
void clientTCP(char * hostname, long port);
void closeConnection(int sockfd);
void listFiles(int sockfd);
#endif // CLIENT_H