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
#include "spp.h"
#define TRAME_SIZE 6
#define BUFFER_SIZE 1024

void handlerChildDeath(){
    wait(NULL);
    printf("[server] le socket applicatif et mort\n");
}

// Fonction de gestion du socket applicatif
void serviceProcess(int serviceSockfd){
    //while first socket charachter equal 
    unsigned char frame[TRAME_SIZE];
    int n;
    while(1){
        bzero(frame, TRAME_SIZE); //bzero permet de vider le buffer et de le remplir de 0
        n = read(serviceSockfd, frame, TRAME_SIZE);
        if (n < 0) {
            perror("[server] Erreur de lecture sur le socket applicatif");
            exit(1);
        }
        if(checkTypeFrame(frame) == 1){
            printf("[server] Trame de type 1 recu\n");
            PDataTrame data = decodeDataHead(frame,6);
            if(data->cmd==0xff){
                SPP_perror("Erreur : le décodage trame");
            }
            
        }
        else if(checkTypeFrame(frame) == 2){
            printf("[server] Trame de type 2 recu\n");
            PInfoTrame inf = decodeInfosTrame(frame,6);
            if(inf->cmd==0xff){
                SPP_perror("Erreur : le décodage trame");
            }
            if(inf->cmd == CLOSE_SOCKET){
                close(serviceSockfd);
                printf("[server] le socket applicatif vas se sucidé\n");
                exit(0);
                break;
            }
        }
        else if(checkTypeFrame(frame) == -1){
            printf("[server] Erreur de type de trame inconue\n");
            exit(-1);
        }
    }
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