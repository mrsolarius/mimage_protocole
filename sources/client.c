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
#include "spp.h"
void print_hex1(const unsigned char *s)
{
    //tant que le pointeur n'est pas à la fin de la chaine
    for(int i = 0; i < MAX_TRAME_SIZE+6; i++)
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
    listFilesC(sockfd);
    closeConnection(sockfd);
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

void listFilesC(int sockfd){
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = GET_LIST;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    int n = write(sockfd, infosTrame, TRAME_SIZE);
    if(n < 0) {
        perror("ERROR pendant la lecture du socket");
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
    unsigned char nbFiles = infosReponse->nbFiles;
    free(bufferList);
    free(infosReponse);

    // lire les noms des fichiers
    printf("nbFiles : %d\n",nbFiles);
    for(int i = 0; i < nbFiles; i++){
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
        printf("File : %s\n",infosReponse->infos);        
    }

}