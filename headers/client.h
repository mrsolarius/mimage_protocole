#ifndef CLIENT_H
#define CLIENT_H
void clientTCP(char * hostname, long port);
void closeConnection(int sockfd);
char ** listFilesC(int sockfd,unsigned char * nbFiles);
void sendGetAndSomthing();
void getFileData(int sockfd, char * fileName);
#endif // CLIENT_H