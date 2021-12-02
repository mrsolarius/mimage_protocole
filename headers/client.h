#ifndef CLIENT_H
#define CLIENT_H
void clientTCP(char * hostname, long port);
void closeConnection(int sockfd);
void listFilesC(int sockfd);
#endif // CLIENT_H