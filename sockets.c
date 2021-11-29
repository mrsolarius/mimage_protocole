//import all required libraries
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void child_process(int serviceSockfd){
    int buffer;
    read(serviceSockfd,&buffer,sizeof(buffer));
    printf("[server] le client dit : %d\n",buffer);

    int n = 6;
    write(serviceSockfd, n, sizeof(n));
    printf("[server] le serveur dit : %d\n",n);
}

void handler_child_death(){
    wait(NULL);
}

void server_tcp(int port){ 
    struct sigaction ac;
    ac.sa_handler = handler_child_death;
    ac.sa_flags = SA_RESTART;

    sigaction(SIGCHLD,&ac, NULL);

    // create tcp socket with precised port
    int listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // define soc structure
    struct sockaddr_in * serv_addr;
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr->sin_port = htons(port);
    
    // bind socket to server address
    if (bind(listenSockfd, serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // listen for incoming connections
    listen(listenSockfd, 5); 

    struct sockaddr_in cli_addr;

    while (0)
    {
        // accept connection
        int serviceSockfd = accept(listenSockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
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
            
            child_process(serviceSockfd);

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


// client_tcp function
void client_tcp(char * hostname, int port) {
    // create tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 1);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    // get server address from hostname
    struct hostent *server;
    struct sockaddr_in serv_addr;

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    

}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <server/client>\n", argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "server") == 0)
    {
        server_tcp(8080);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        client_tcp("localhost",8080);
    }
    else
    {
        printf("Usage: %s <server/client>\n", argv[0]);
        exit(1);
    }
    return 0;
}