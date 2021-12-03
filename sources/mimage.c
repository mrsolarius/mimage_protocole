//import all required libraries
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

#include "client.h"
#include "server.h"

//Main that call server_tcp or client_tcp function depending on the argument given in the command line
int main(int argc, char *argv[]){
    long *port = malloc(sizeof(long));
    if(argc!=3 && argc!=4){
        printf("Usage : %s server <port>\n",argv[0]);
        printf("Usage : %s client <hostname> <port>\n",argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "server") == 0)
    {
        if(argc!=3){
            printf("Usage : %s server <port>\n",argv[0]);
            exit(1);
        }
        *port = strtol(argv[2],NULL,10);
        if(*port<1 || *port>65535){
            printf("Le port doit être compris entre 1 et 65535\n");
            printf("Usage : %s server <port>\n",argv[0]);
            exit(1);
        }
        serverTCP(*port);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        if(argc!=4){
            printf("Usage : %s client <hostname> <port>\n",argv[0]);
            exit(1);
        }
        *port = strtol(argv[3],NULL,10);
        if(*port<1 || *port>65535){
            printf("Le port doit être compris entre 1 et 65535\n");
            printf("Usage : %s client <adresse> <port>\n",argv[0]);
            exit(1);
        }
        clientTCP(argv[2],*port);
    }
    else
    {
        printf("Usage : %s <server/client> <adresse> <port>\n", argv[0]);
        exit(1);
    }
    return 0;
}