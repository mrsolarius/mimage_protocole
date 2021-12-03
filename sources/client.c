#include "client.h"
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "spp.h"
void print_hex1(const unsigned char *s)
{
    //tant que le pointeur n'est pas à la fin de la chaine
    for(int i = 0; i < 6; i++)
        printf("%02x-", s[i]);
    
    printf("\n");
}
// fonction principal du client
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
    memcpy(&serv_addr->sin_addr.s_addr,server->h_addr,15); //15 corespond à la taille maximal d'une adresse ipv4
    serv_addr->sin_port = htons(port);
    // connect to server
    int conn = connect(sockfd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr));
    if (conn < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    free(serv_addr);
    // send message to server
    //sendGetAndSomthing(sockfd);
    //getFileData(sockfd,"1584641959_264125101_0.gif");
    sendFile(sockfd,"t430_v4.ico");
    closeConnection(sockfd);
}

void sendGetAndSomthing(int sockfd){
    unsigned char * taille = malloc(sizeof(unsigned char));
    char * * list = listFilesC(sockfd, taille);
    //print list char without know size
    for(int i = 0; i < *taille; i++)
        printf("%s\n",list[i]);
    free(taille);
}

// fonction de fermeture de la connexion
void closeConnection(int sockfd) {
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = CLOSE_SOCKET;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    
    int n = write(sockfd, infosTrame, 6);
    if(n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    // close connection
    close(sockfd);
    printf("[client] fermeture de la connexion\n");
    exit(0);
}

char ** listFilesC(int sockfd,unsigned char * nbFiles) {
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = GET_LIST;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    int n = write(sockfd, infosTrame, TRAME_SIZE);
    if(n < 0) {
        perror("ERROR pendant l'écriture du socket");
        exit(1);
    }
    free(infosTrame);
    free(infos);

    // lire la réponse du serveur pour savoir le nombre de fichiers
    unsigned char* bufferList = (unsigned char*) malloc(TRAME_SIZE);
    n = read(sockfd, bufferList, TRAME_SIZE);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    PInfoTrame infosReponse = decodeInfosTrame(bufferList);
    *nbFiles = infosReponse->nbFiles;
    free(bufferList);
    free(infosReponse);

    // lire les noms des fichiers
    char ** files = (char **) malloc(sizeof(char *) * *nbFiles);
    for(int i = 0; i < *nbFiles; i++){
        unsigned char * buffer = (unsigned char*) malloc(TRAME_SIZE);
        n = read(sockfd, buffer, TRAME_SIZE);
        if(n < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
        PInfoTrame infosReponse = decodeInfosTrame(buffer);
        printf("size : %d\n",infosReponse->sizeInfos);
        
        unsigned char * bufferInfo = (unsigned char*) malloc(infosReponse->sizeInfos);
        n = read(sockfd, bufferInfo, infosReponse->sizeInfos);
        decodeInfosTrame_Infos(infosReponse,bufferInfo,infosReponse->sizeInfos);
        //printf("File : %s\n",infosReponse->infos);
        files[i] = (char *) malloc(sizeof(char) * (infosReponse->sizeInfos+1)); 
        strcpy(files[i],infosReponse->infos);  
        free(infosReponse);
        free(bufferInfo);        
    }
    return files;
}

void getFileData(int sockfd, char * fileName) {
    // On crée la trame de demande de fichier
    int datafd = open(fileName, O_WRONLY | O_CREAT, 0666);
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = GET_FILE_DATA;
    infos->status = SUCCESS;
    infos->sizeInfos = strlen(fileName);
    infos->nbFiles = 0;
    infos->infos = (char *) malloc(sizeof(char) * (infos->sizeInfos+1));
    strcpy(infos->infos,fileName);
    unsigned char* infosTrame = encodeInfosTrame(infos);
    int n = write(sockfd, infosTrame, TRAME_SIZE+infos->sizeInfos);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    free(infosTrame);
    // on
    unsigned char* bufferDataHead = (unsigned char*) malloc(TRAME_SIZE);
    n = read(sockfd, bufferDataHead, TRAME_SIZE);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    printf("info info %s",infos->infos);
    // création d'un filedescriptor
    if(datafd == -1){
        perror("ERROR pendant la création du fichier");
        exit(1);
    }
    PDataTrame dataHead = decodeDataHead(bufferDataHead, datafd);
    // On test le status de la réponse
    if(dataHead->status == NO_FOUND_FILE){
        printf("[client] fichier non trouvé\n");
        free(dataHead);
        free(bufferDataHead);
        return;
    }else if (dataHead->status == INTERNAL_ERROR){
        printf("[client] erreur inconnue c'est produite\n");
        free(dataHead);
        free(bufferDataHead);
        return;
    }
    char tampon[BUFFER_SIZE];
    printf("sizeData %d",dataHead->sizeData);
    //Parcours du socket tant qu'on n'est pas arrivé au bout ou qu'il n'y a pas eu une erreur
    for(int i = 0;  i< dataHead->sizeData; i+=BUFFER_SIZE){
        printf("octets lu : %d\n",dataHead->sizeData-i);
        if(i+BUFFER_SIZE > dataHead->sizeData){
            n = read(sockfd, tampon, dataHead->sizeData-i);
            write(datafd, tampon, dataHead->sizeData-i);
            bzero(tampon,  BUFFER_SIZE);
        }else{
            n = read(sockfd, tampon, BUFFER_SIZE);
            write(datafd, tampon, BUFFER_SIZE);
            bzero(tampon,  BUFFER_SIZE);
        }
        if(n < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
    }
    free(bufferDataHead);
}


void sendFile(int sockfd, char * fileName){
    // On crée la trame de demande de fichier
    printf("[client] envoi du fichier %s\n",fileName);
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = UPLOAD_FILE_NAME;
    infos->status = SUCCESS;
    infos->sizeInfos = strlen(fileName);
    infos->nbFiles = 0;
    infos->infos = (char *) malloc(sizeof(char) * (infos->sizeInfos+1));
    strcpy(infos->infos,fileName);
    unsigned char* infosTrame = encodeInfosTrame(infos);
    print_hex1(infosTrame);
    int e = write(sockfd, infosTrame, TRAME_SIZE+infos->sizeInfos);
    if(e < 0) {
        perror("ERROR pendant l'écriture du socket");
        exit(1);
    }
    free(infos);
    free(infosTrame);

    // On récécéptione la trame d'aquitement
    unsigned char* acquitFrame = (unsigned char*) malloc(TRAME_SIZE);
    e = read(sockfd, acquitFrame, TRAME_SIZE);
    print_hex1(acquitFrame);
    if(e < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    PInfoTrame acquit = decodeInfosTrame(acquitFrame);
    printf("[client] cmd : %x\n",acquit->cmd);
    printf("[client] acquit : %x\n",acquit->status);
    free(acquitFrame);

    if(acquit->status==SUCCESS){
        // On ouvre le fichier à envoyer
        int datafd = open(fileName, O_RDONLY);
        // on stock la taille du fichier dans une variable
        struct stat fileStat;
        fstat(datafd, &fileStat);
        long sizeFile = fileStat.st_size;
        printf("[client] taille du fichier : %ld\n",sizeFile);

        // On crée l'entête de la trame de données
        PDataTrame dataHead = (PDataTrame) malloc(sizeof(PDataTrame));
        dataHead->cmd = UPLOAD_FILE_DATA;
        dataHead->status = SUCCESS;
        dataHead->sizeData = sizeFile;
        dataHead->dataFd = datafd;
        unsigned char* dataHeadTrame = encodeDataHead(dataHead);
        print_hex1(dataHeadTrame);
        printf("SPP erno %d\n", SPP_Erno);
        // On envoie l'entête de la trame de données
        e = write(sockfd, dataHeadTrame, TRAME_SIZE);
        if(e < 0) {
            perror("ERROR pendant l'écriture du socket");
            exit(1);
        }
        free(dataHeadTrame);
        // On envoie les données du fichier
        char tampon[BUFFER_SIZE];
        for(int i = 0;  i < dataHead->sizeData; i+=BUFFER_SIZE){
            printf("octets lu : %d\n",dataHead->sizeData-i);
            if(i+BUFFER_SIZE > dataHead->sizeData){
                read(dataHead->dataFd, tampon,  dataHead->sizeData-i);
                e = write(sockfd, tampon, dataHead->sizeData-i);
                bzero(tampon,  dataHead->sizeData-i);
            }else{
                read(dataHead->dataFd, tampon,  BUFFER_SIZE);
                e = write(sockfd, tampon, BUFFER_SIZE);
                bzero(tampon,  BUFFER_SIZE);
            }
            if(e < 0) {
                perror("ERROR pendant l'écriture du socket");
                exit(1);
            }
        }
        close(datafd);
        free(dataHead);

        //ATTENTION LECTURE A FAIRE ET RENVOYER UNE ERREUR
    }





























    /*
    // On crée la trame de demande de fichier
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = UPLOAD_FILE_NAME;
    infos->status = SUCCESS;
    infos->sizeInfos = strlen(fileName);
    infos->infos = fileName;
    printf("infos : %s size: %d\n",infos->infos,infos->sizeInfos);
    unsigned char* infosTrame = encodeInfosTrame(infos);
    // On envoie la trame au serveur
    printf("[client] envoie de la trame d'upload au serveur\n");
    int n = write(sockfd, infosTrame, TRAME_SIZE+infos->sizeInfos);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    free(infosTrame);

    //Lecture de l'aquitement de la demande de fichier
    unsigned char* bufferList = (unsigned char*) malloc(TRAME_SIZE);
    printf("[client] en aquitement de la réponse\n");
    n = read(sockfd, bufferList, BUFFER_SIZE);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    printf("bufferList[0] : %x\n",bufferList[0]);
    print_hex1(bufferList);
    PInfoTrame infosReponse = decodeInfosTrame(bufferList);
    printf("spp_erno : %d\n",SPP_Erno);
    //@todo géré les cas d'erreurs
    printf("cmd: %x status : %x\n",infosReponse->cmd, infosReponse->status);
    *//*
    if(infosReponse->status == SUCCESS){
        printf("[client] le fichier %s peut être envoyé\n",fileName);
        FILE * f = fopen(fileName, "rb");
        //get size of file in bytes in size var
        fseek(f, 0, SEEK_END); // seek to end of file
        int size = ftell(f); // get current file pointer
        fseek(f, 0, SEEK_SET);
        int datafd = open(fileName, O_RDONLY);
        if(datafd == -1){
            perror("ERROR pendant la création du fichier");
            exit(1);
        }

        //on crée la trame de données
        PDataTrame data = (PDataTrame) malloc(sizeof(PDataTrame));
        data->cmd = UPLOAD_FILE_DATA;
        data->status = SUCCESS;
        data->sizeData = size;
        unsigned char* dataTrame = encodeDataHead(data);

        //on envoie l'entête de données
        printf("[client] envoie de l'entête de données\n");
        n = write(sockfd, dataTrame, TRAME_SIZE);
        if(n < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
        free(dataTrame);

        char tampon[BUFFER_SIZE];
        printf("[server] envoie du fichier %d\n",data->sizeData);
        //Parcours du socket tant qu'on n'est pas arrivé au bout ou qu'il n'y a pas eu une erreur
        for(int i = 0;  i < data->sizeData; i+=BUFFER_SIZE){
            printf("octets lu : %d\n",data->sizeData-i);
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
        
        //on lis la réponse du serveur
        unsigned char* bufferData = (unsigned char*) malloc(TRAME_SIZE);
        n = read(sockfd, bufferData, TRAME_SIZE);
        if(n < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
        PInfoTrame infoHead = decodeInfosTrame(bufferData);
        // On test le status de la réponse
        if(infoHead->status == SUCCESS){
            printf("[client] le fichier %s a été envoyé\n",fileName);
        }
    }*/
}