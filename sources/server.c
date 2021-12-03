#include "server.h"
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include "test-core.h"
#include "utils.h"
void print_hex_0(const unsigned char *s)
{
    for(int i = 0; i < MAX_TRAME_SIZE+6; i++)
        printf("%02x-", s[i]);
    
    printf("\n");
}
void handlerChildDeath(){
    wait(NULL);
    printf("[server] le socket applicatif est mort\n");
}

// Fonction de gestion du socket applicatif
void serviceProcess(int serviceSockfd){
    //while first socket charachter equal 
    unsigned char frame[TRAME_SIZE];
    int n;
    while(1){
        bzero(frame, TRAME_SIZE); //bzero permet de vider le buffer et de le remplir de 0
        n = read(serviceSockfd, frame, TRAME_SIZE);
        //print_hex_0(frame);
        if (n < 0) {
            perror("[server] Erreur de lecture sur le socket applicatif");
            exit(1);
        }
        if(checkTypeFrame(frame) == 1){
            printf("[server] Trame de type DataFrame recu\n");
            PDataTrame data = decodeDataHead(frame,6);
            if(data->cmd==0xff){
                SPP_perror("Erreur : le décodage trame");
            }
            
        }
        else if(checkTypeFrame(frame) == 2){
            printf("[server] Trame de type InfoFrame recu\n");
            PInfoTrame inf = decodeInfosTrame(frame);
            if(inf->cmd==0xff){
                SPP_perror("Erreur : le décodage trame");
            }
            if(inf->cmd == CLOSE_SOCKET){
                close(serviceSockfd);
                printf("[server] le socket applicatif va se sucider\n");
                exit(0);
                break;
            }
            if(inf->cmd== GET_LIST){
                printf("[server] demande de liste\n");
                listFilesS(serviceSockfd);
            }
            if(inf->cmd == GET_FILE_DATA){
                printf("[server] demande de fichier\n");
                downloadFileS(serviceSockfd, inf);
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

void listFilesS(int sockfd){
    int count_Files = countFiles("tests/types/");
    PInfoTrame inf = (PInfoTrame)malloc(sizeof(PInfoTrame));
    inf->cmd = LIST_SIZE;
    inf->status = SUCCESS;
    inf->sizeInfos = 0;
    inf->nbFiles = count_Files;
    inf->infos = NULL;

    unsigned char * frame = encodeInfosTrame(inf);
    int n = write(sockfd, frame, TRAME_SIZE);
    if (n < 0) {
        perror("[server] Erreur de lecture sur le socket applicatif");
        exit(1);
    }
    free(frame);
    free(inf);

    char * * files = getFiles("tests/types/",count_Files);
    for(int i = 0; i < count_Files; i++){
        PInfoTrame info = (PInfoTrame)malloc(sizeof(InfosTrame));
        info->cmd = DOWNLOAD_FILE_NAME;
        info->status = SUCCESS;
        info->sizeInfos = strlen(files[i]);
        info->nbFiles = 1;
        info->infos = files[i];
        frame = encodeInfosTrame(info);
        printf("[server] envoie %s\n",info->infos);
        //print_hex_0(frame);
        int n = write(sockfd, frame, TRAME_SIZE+info->sizeInfos);
        if (n < 0) {
            perror("[server] Erreur de lecture sur le socket applicatif");
            exit(1);
        }
        free(frame);
        free(info->infos);
        free(info);
    }
}

void downloadFileS(int sockfd, PInfoTrame info) {
    unsigned char * bufferInfo = (unsigned char*) malloc(info->sizeInfos);
    int n = read(sockfd, bufferInfo, info->sizeInfos);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    decodeInfosTrame_Infos(info,bufferInfo,info->sizeInfos);
    free(bufferInfo);
    printf("[server] demande de fichier %s\n",info->infos);
    
    // open file
    //concat two string
    char * path = (char*)malloc(strlen("tests/types/")+strlen(info->infos)+1);
    strcpy(path,"tests/types/");
    strcat(path,info->infos);
    FILE * f = fopen(path, "rb");
    //get size of file in bytes in size var
    fseek(f, 0, SEEK_END); // seek to end of file
    int size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file
    int datafd = open(path, O_RDONLY);
    PDataTrame data = (PDataTrame)malloc(sizeof(DataTrame));
    data->cmd = DOWNLOAD_FILE_DATA;
    data->status = SUCCESS;
    data->sizeData = size;
    data->dataFd = datafd;
    unsigned char * dataHead = malloc(TRAME_SIZE);
    dataHead = encodeDataHead(data);
    printf("zaetta\n");
    printf("[server] envoie du fichier %d\n",data->sizeData);
    int n2 = write(sockfd, dataHead, TRAME_SIZE);
    if (n2 < 0) {
        perror("ERROR pendant l'écriture du socket");
        exit(1);
    }
    char tampon[BUFFER_SIZE];
    printf("[server] envoie du fichier %d\n",data->sizeData);
    //Parcours du socket tant qu'on n'est pas arrivé au bout ou qu'il n'y a pas eu une erreur
    for(int i = 0;  i < data->sizeData; i+=BUFFER_SIZE){
        printf("octets lu : %d\n",data->sizeData-i);
        
        if(i+BUFFER_SIZE > data->sizeData){
            read(datafd, tampon,  data->sizeData-i);
            print_hex_0(tampon);
            n = write(sockfd, tampon, data->sizeData-i);
            bzero(tampon,  data->sizeData-i);
        }else{
            read(datafd, tampon,  BUFFER_SIZE);
            print_hex_0(tampon);
            n = write(sockfd, tampon, BUFFER_SIZE);
            bzero(tampon,  BUFFER_SIZE);
        }
        if(n < 0) {
            perror("ERROR pendant l'écriture du socket");
            exit(1);
        }
    }
    free(dataHead);
    free(data);
    free(info->infos);
    free(info);
}