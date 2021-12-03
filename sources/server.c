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
    for(int i = 0; i < 6; i++)
        printf("%02x-", s[i]);
    
    printf("\n");
}
void handlerChildDeath(){
    wait(NULL);
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
            if(inf->cmd == UPLOAD_FILE_NAME){
                printf("[server] demande d'envoie de fichier\n");
                uploadFile(serviceSockfd, inf);
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
    
    int datafd = open(path, O_RDONLY);
    // on stock la taille du fichier dans une variable
    struct stat fileStat;
    fstat(datafd, &fileStat);
    long size = fileStat.st_size;

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
        printf("Octets restants : %d\n",data->sizeData-i);
        
        if(i+BUFFER_SIZE > data->sizeData){
            read(datafd, tampon,  data->sizeData-i);
            n = write(sockfd, tampon, data->sizeData-i);
            bzero(tampon,  data->sizeData-i);
        }else{
            read(datafd, tampon,  BUFFER_SIZE);
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

void uploadFile(int socketfd, PInfoTrame info){
    //On lit le nom du fichier dans upload file info
    unsigned char * bufferInfo = (unsigned char*) malloc(info->sizeInfos);
    int e = read(socketfd, bufferInfo, info->sizeInfos);
    printf("[server] taille de fichier %d\n",info->sizeInfos);
    if(e < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    decodeInfosTrame_Infos(info,bufferInfo,info->sizeInfos);
    free(bufferInfo);

    //On ouvre le fichier
    char * path = (char*)malloc(strlen("tests/types/")+strlen(info->infos)+1);
    strcpy(path,"tests/types/");
    strcat(path,info->infos);
    int dataFd = open(path, O_WRONLY | O_CREAT, 0666);
    if(dataFd < 0){
        perror("[server] Erreur d'ouverture/création du fichier");
        exit(1);
    }

    //On envoie la trame Acquit File Info (sans verification pour le momment)
    PInfoTrame acquitInf = (PInfoTrame)malloc(sizeof(InfosTrame));
    acquitInf->cmd = ACQUIT_FILE_NAME;
    acquitInf->status = SUCCESS;
    acquitInf->sizeInfos = 0;
    acquitInf->nbFiles = 0;
    unsigned char * acquitFrame = encodeInfosTrame(acquitInf);
    int n = write(socketfd, acquitFrame, TRAME_SIZE);
    printf("[server] envoie acquit\n");
    if (n < 0) {
        perror("[server] Erreur d'écriture sur le socket applicatif");
        exit(1);
    }
    free(acquitFrame);
    free(acquitInf);

    printf("[server] demande d'upload du fichier %s\n",info->infos);
    
    //On lit l'entête de la trame data
    unsigned char * dataHeadFrame = malloc(TRAME_SIZE);
    n = read(socketfd, dataHeadFrame, TRAME_SIZE);
    PDataTrame dataHead = decodeDataHead(dataHeadFrame,dataFd);
    printf("[server] taille du fichier à recevoir %d\n",dataHead->sizeData);
    free(dataHeadFrame);

    //On lit les données reçues et on les écrit dans le fichier
    char tampon[BUFFER_SIZE];
    for(int i = 0;  i< dataHead->sizeData; i+=BUFFER_SIZE){
        printf("Octets restants : %d\n",dataHead->sizeData-i);
        if(i+BUFFER_SIZE > dataHead->sizeData){
            n = read(socketfd, tampon, dataHead->sizeData-i);
            write(dataHead->dataFd, tampon, dataHead->sizeData-i);
            bzero(tampon,  BUFFER_SIZE);
        }else{
            n = read(socketfd, tampon, BUFFER_SIZE);
            write(dataHead->dataFd, tampon, BUFFER_SIZE);
            bzero(tampon,  BUFFER_SIZE);
        }
        if(n < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
    }
    free(dataHead);
    close(dataFd);

    // On verifie le type du fichier
    char type[255];
    strcpy(type,getType(path));
    printf("[server] type du fichier %s\n",type);
    free(path);

    int size;
    char ** fileArray = fileToArray("MimeTypes.txt",&size);

    //On inisialise la trame ACQUIT_FILE_DATA
    PInfoTrame acquitData = (PInfoTrame)malloc(sizeof(InfosTrame));
    acquitData->cmd = ACQUIT_FILE_DATA;
    acquitData->nbFiles = 0;
    acquitData->sizeInfos = 0;

    //On verifie que le type du fichier et conforme au attendu du serveur
    if(isInArray(type,fileArray,size)){
        acquitData->status = SUCCESS;
        printf("[server] type du fichier valide\n");
    }else{
        acquitData->status = INVALID_EXTEND_FILE;
        printf("[server] type du fichier invalide\n");
    }

    //On encode la trame ACQUIT_FILE_DATA
    unsigned char * acquitDataFrame = encodeInfosTrame(acquitData);
    //On envoie la trame ACQUIT_FILE_DATA
    n = write(socketfd, acquitDataFrame, TRAME_SIZE);
    printf("[server] envoie acquit\n");
    if (n < 0) {
        perror("[server] Erreur d'écriture sur le socket applicatif");
        exit(1);
    }
    free(acquitDataFrame);
    free(acquitData);
}