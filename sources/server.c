#include "server.h"
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>

void handlerChildDeath(){
    wait(NULL);
    printf("[server] le socket applicatif et mort\n");
}

// Fonction de gestion du socket applicatif
void serviceProcess(int serviceSockfd){

    int buffer;
    read(serviceSockfd,&buffer,sizeof(buffer));
    printf("[server] le client dit : %d\n",buffer);

    int n = 6;
    write(serviceSockfd, &n, sizeof(n));
    printf("[server] le serveur dit : %d\n",n);

    read(serviceSockfd,&buffer,sizeof(buffer));
    printf("[server] le client dit : %d\n",buffer);
}

void serverTCP(int port){
    printf("[server] le serveur s'ouvre sur le port %d\n",port);

    struct sigaction ac;
    ac.sa_handler = handlerChildDeath;
    ac.sa_flags = SA_RESTART;
    printf("[server] le serveur est en marche\n");
    sigaction(SIGCHLD,&ac, NULL);
    

    // create tcp socket with precised port
    int listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    printf("[server] socket created\n");
    // define soc structure
    struct sockaddr_in  serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    // bind socket to server address
    if (bind(listenSockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    printf("[server] socket binded\n");
    // listen for incoming connections
    listen(listenSockfd, 5); 
    printf("[server] socket listen\n");

    struct sockaddr_in * cli_addr = NULL;
    while (1)
    {
        printf("[server] waiting for incoming connection\n");
        // accept connection
        socklen_t * size = (socklen_t *) malloc(sizeof(socklen_t));
        int serviceSockfd = accept(listenSockfd, (struct sockaddr *) cli_addr, size);
        if (serviceSockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // create child process to handle request
        int pid = fork();
        if(pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }
        else if(pid == 0) {
            // child process
            close(listenSockfd);

            serviceProcess(serviceSockfd);

            // close connection
            close(serviceSockfd);
            exit(0);
        }
        else {
            // parent process
            close(serviceSockfd);
        }
    }
    

}