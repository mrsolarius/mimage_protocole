#include "client.h"
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// client_tcp function
void clientTCP(char * hostname, long port) {
    printf("[client] le client se connecte au serveur %s sur le port %ld\n",hostname,port);
    // create tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    // get server address from hostname
    struct hostent * server;
    struct sockaddr_in * serv_addr = malloc(sizeof(struct sockaddr_in));

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr->sin_family = AF_INET;
    memcpy(&serv_addr->sin_addr.s_addr,server->h_addr,15);
    serv_addr->sin_port = htons(port);
    // connect to server
    int conn = connect(sockfd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr));
    if (conn < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    free(serv_addr);
    // send message to server
    int n = 5;
    write(sockfd, &n, sizeof(n));
    printf("[client] le client dit : %d\n",n);

    sleep(20);

    n = 4;
    write(sockfd, &n, sizeof(n));
    printf("[client] le client dit : %d\n",n);
    // read message from server
    int buffer;
    read(sockfd,&buffer,sizeof(buffer));
    printf("[client] le serveur dit : %d\n",buffer);

    // close connection
    close(sockfd);
}