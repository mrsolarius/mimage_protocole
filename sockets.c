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

//Make while loop that close when they want
void child_process(int serviceSockfd){   
    int buffer;
    read(serviceSockfd,&buffer,sizeof(buffer));
    printf("[server] le client dit : %d\n",buffer);

    int n = 6;
    write(serviceSockfd, &n, sizeof(n));
    printf("[server] le serveur dit : %d\n",n);

    buffer;
    read(serviceSockfd,&buffer,sizeof(buffer));
    printf("[server] le client dit : %d\n",buffer);
}

void handler_child_death(){
    wait(NULL);
    printf("JE MEURS !!!!;\n");
}

void server_tcp(int port){ 
    printf("[server] le serveur s'ouvre sur le port %d\n",port);

    struct sigaction ac;
    ac.sa_handler = handler_child_death;
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

    struct sockaddr_in * cli_addr;
    while (1)
    {
        printf("[server] waiting for incoming connection\n");
        // accept connection
        int size;
        int serviceSockfd = accept(listenSockfd, (struct sockaddr *) cli_addr, &size);
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
    printf("[client] le client se connecte au serveur %s sur le port %d\n",hostname,port);
    // create tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    // get server address from hostname
    struct hostent * server;
    struct sockaddr_in * serv_addr;

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr->sin_family = AF_INET;
    memcpy(&serv_addr->sin_addr.s_addr,server->h_addr,sizeof(server->h_addr));
    serv_addr->sin_port = htons(port);
    // connect to server
    int conn = connect(sockfd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr));
    if (conn < 0) {
        perror("ERROR connecting");
        exit(1);
    }

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

//Main that call server_tcp or client_tcp function depending on the argument given in the command line
int main(int argc, char *argv[]){
    if (strcmp(argv[1], "server") == 0)
    {
        if(argc != 3) {
            printf("Usage: %s server port\n", argv[0]);
            exit(1);
        }
        server_tcp(6555);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        if(argc != 4) {
            printf("Usage: %s client hostname port\n", argv[0]);
            exit(1);
        }
        
        client_tcp(argv[2],6555);
    }
    else
    {
        printf("Usage: %s <server/client> <adresse> <port>\n", argv[0]);
        exit(1);
    }
    return 0;
}