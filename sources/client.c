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
#include "interface.h"

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
    Affiche(sockfd);
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
    printf("\n[client] fermeture de la connexion\n");
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
    char * path = (char*)malloc(strlen(PATH_CLIENT)+strlen(fileName)+1);
    strcpy(path,PATH_CLIENT);
    strcat(path,fileName);
    int datafd = open(path, O_WRONLY | O_CREAT, 0666);
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
    //printf("info info %s",infos->infos);
    // création d'un filedescriptor
    if(datafd == -1){
        perror("ERROR pendant la création du fichier");
        exit(1);
    }
    PDataTrame dataHead = decodeDataHead(bufferDataHead, datafd);
    // On test le status de la réponse
    if(dataHead->status == NO_FOUND_FILE){
        printf("\n[client] fichier non trouvé\n");
        free(dataHead);
        free(bufferDataHead);
        return;
    }else if (dataHead->status == INTERNAL_ERROR){
        printf("\n[client] erreur inconnue c'est produite\n");
        free(dataHead);
        free(bufferDataHead);
        return;
    }
    char tampon[BUFFER_SIZE];
    //printf("sizeData %d",dataHead->sizeData);
    //Parcours du socket tant qu'on n'est pas arrivé au bout ou qu'il n'y a pas eu une erreur
    for(int i = 0;  i< dataHead->sizeData; i+=BUFFER_SIZE){
        //printf("octets lu : %d\n",dataHead->sizeData-i);
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


unsigned char sendFile(int sockfd, char * fileName) {
    // On crée la trame de demande de fichier
    // On prépare le path
    char * path = (char*)malloc(strlen(PATH_CLIENT)+strlen(fileName)+1);
    strcpy(path,PATH_CLIENT);
    strcat(path,fileName);
    // On ouvre le fichier à envoyer avec access pour ne pas créer de fichier
    int datafd = open(path, O_RDONLY);
    if (datafd == -1) {
        perror("ERROR le fichier demander n'existe pas");
        return NO_FOUND_FILE;
    }
    // On crée la trame de demande de fichier
    printf("[client] envoi du fichier %s\n", fileName);
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = UPLOAD_FILE_NAME;
    infos->status = SUCCESS;
    infos->sizeInfos = strlen(fileName);
    infos->nbFiles = 0;
    infos->infos = (char *) malloc(sizeof(char) * (infos->sizeInfos + 1));
    strcpy(infos->infos, fileName);
    unsigned char *infosTrame = encodeInfosTrame(infos);
    int e = write(sockfd, infosTrame, TRAME_SIZE + infos->sizeInfos);
    if (e < 0) {
        perror("ERROR pendant l'écriture du socket");
        exit(1);
    }
    free(infos);
    free(infosTrame);

    // On récécéptione la trame d'aquitement
    unsigned char *acquitFrame = (unsigned char *) malloc(TRAME_SIZE);
    e = read(sockfd, acquitFrame, TRAME_SIZE);
    if (e < 0) {
        perror("ERROR pendant la lecture du socket");
        exit(1);
    }
    PInfoTrame acquit = decodeInfosTrame(acquitFrame);
    free(acquitFrame);

    if (acquit->status == SUCCESS) {
        // on stock la taille du fichier dans une variable
        struct stat fileStat;
        fstat(datafd, &fileStat);
        long sizeFile = fileStat.st_size;

        // On crée l'entête de la trame de données
        PDataTrame dataHead = (PDataTrame) malloc(sizeof(PDataTrame));
        dataHead->cmd = UPLOAD_FILE_DATA;
        dataHead->status = SUCCESS;
        dataHead->sizeData = sizeFile;
        dataHead->dataFd = datafd;
        unsigned char *dataHeadTrame = encodeDataHead(dataHead);
        // On envoie l'entête de la trame de données
        e = write(sockfd, dataHeadTrame, TRAME_SIZE);
        if (e < 0) {
            perror("ERROR pendant l'écriture du socket");
            exit(1);
        }
        free(dataHeadTrame);
        // On envoie les données du fichier
        char tampon[BUFFER_SIZE];
        for (int i = 0; i < dataHead->sizeData; i += BUFFER_SIZE) {
            if (i + BUFFER_SIZE > dataHead->sizeData) {
                read(dataHead->dataFd, tampon, dataHead->sizeData - i);
                e = write(sockfd, tampon, dataHead->sizeData - i);
                bzero(tampon, dataHead->sizeData - i);
            } else {
                read(dataHead->dataFd, tampon, BUFFER_SIZE);
                e = write(sockfd, tampon, BUFFER_SIZE);
                bzero(tampon, BUFFER_SIZE);
            }
            if (e < 0) {
                perror("ERROR pendant l'écriture du socket");
                exit(1);
            }
        }
        close(datafd);
        free(dataHead);

        // On récécéptione la trame d'aquitement
        unsigned char *acquitFrame = (unsigned char *) malloc(TRAME_SIZE);
        e = read(sockfd, acquitFrame, TRAME_SIZE);
        if (e < 0) {
            perror("ERROR pendant la lecture du socket");
            exit(1);
        }
        PInfoTrame acquit = decodeInfosTrame(acquitFrame);
        unsigned char returnStatus = acquit->status;
        free(acquitFrame);
        return returnStatus;
    }
    return INTERNAL_ERROR;
}