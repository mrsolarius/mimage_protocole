#ifndef CLIENT_H
#define CLIENT_H
#define PATH_CLIENT "data/client/"
void clientTCP(char * hostname, long port);
void closeConnection(int sockfd);
char ** listFilesC(int sockfd,unsigned char * nbFiles);
void sendGetAndSomthing(int sockfd);
void getFileData(int sockfd, char * fileName);
unsigned char sendFile(int sockfd, char * fileName);
#endif // CLIENT_H