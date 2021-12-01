#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "spp.h"

int SPP_Erno=-1;
const char *DDP_errList[] = {
    //@todo: add error messages
};

bool checkInfoTrameError(PInfoTrame infosTrame){
    bool test=false;
    if (infosTrame->cmd == 0)
        {
        SPP_Erno=EMPTY_CMD;
        test=true;
    }else
    if (((infosTrame->cmd<0xA1)||(infosTrame->cmd>0xCE))||
        ((infosTrame->cmd>0xA4)&&(infosTrame->cmd<0xC1))||
        ((infosTrame->cmd>0xC3)&&(infosTrame->cmd<0xCE))){
        SPP_Erno=CMD_ERROR;
        test=true;
    }else
    if (infosTrame->status==0){
        SPP_Erno=EMPTY_STATUS;
        test=true;
    }else
    if (((infosTrame->status<0x20)||(infosTrame->status>0x52))||
        ((infosTrame->status>0x20)&&(infosTrame->status<0x41))||
        ((infosTrame->status>0x45)&&(infosTrame->status<0x50))){
        SPP_Erno=STATUS_ERROR;
        test=true;
    }else
    if ((infosTrame->sizeInfos) != (strlen(infosTrame->infos))){
        SPP_Erno=WRONG_SIZE;
        test=true;
    }
    return test;
}

bool checkDataTrameError(PDataTrame dataTrame){
    bool test=false;
    if (dataTrame->cmd==0){
        SPP_Erno=EMPTY_CMD;
        test=true;
    }
    if ((dataTrame->cmd!=DOWNLOAD_FILE_DATA)||(dataTrame->cmd!=UPLOAD_FILE_DATA)){
        SPP_Erno=CMD_ERROR;
        test=true;
    }
    if (dataTrame->status==0){
        SPP_Erno=EMPTY_STATUS;
        test=true;
    }
    if ((dataTrame->status!=SUCCESS)||
        !((dataTrame->status>=0x41)&&(dataTrame->status<=0x45))||
        !((dataTrame->status>=0x50)&&(dataTrame->status<=0x52))){
        SPP_Erno=STATUS_ERROR;
        test=true;
        }
    if (dataTrame->dataFd==0){
        SPP_Erno=EMPTY_FD;
        test=true;
    }
    if (dataTrame->dataFd<0){
        SPP_Erno=FD_ERROR;
        test=true;
    }
    if ((dataTrame->status==SUCCESS)&&(dataTrame->sizeData==0)){
        SPP_Erno=EMPTY_SIZE_DATA;
        test=true;
    }
    return test;
}

char* encodeInfosTrame(PInfoTrame infosTrame){
    // on check tous les problèmes potentiel pouvant subevenir
    if (checkInfoTrameError(infosTrame)==true){
        return (char *) (0xFF);
    }
    //on met en place la structure de la trame après l'avoir déclarée
    char * trame= malloc(sizeof(char)*4+strlen(infosTrame->infos));
    trame[0]=infosTrame->cmd;
    trame[1]=infosTrame->status;
    trame[2]=infosTrame->nbFiles;
    trame[3]=infosTrame->sizeInfos;
    for (int comp=0;comp<infosTrame->sizeInfos;comp++){
        trame[comp+4]=infosTrame->infos[comp];
    }
    return trame;
}

PInfoTrame decodeInfosTrame(char* infos, unsigned int size){
    PInfoTrame TrameDecod = (PInfoTrame) malloc(sizeof(InfosTrame));
    TrameDecod->cmd=infos[0];
    TrameDecod->status=infos[1];
    TrameDecod->nbFiles=infos[2];
    TrameDecod->sizeInfos=infos[3];
    TrameDecod->infos = malloc(sizeof(char)*(TrameDecod->sizeInfos));
    for (int comp=0;comp<TrameDecod->sizeInfos;comp++){
        TrameDecod->infos[comp]=infos[comp+4];
    }
    return TrameDecod;
}

char* encodeDataHead(PDataTrame dataTrame){
        // on check tous les problèmes potentiel pouvant subevenir
    if (checkDataTrameError(dataTrame)==true){
        return (char *) (0xFF);
    }
     char * trame= malloc(sizeof(char)*6); //soit la taille de 2 char et d'un long
    trame[0]=dataTrame->cmd;
    trame[1]=dataTrame->status;
    trame[2]=(dataTrame->sizeData>>24)& 0xFF; // On décalle de 24 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[3]=(dataTrame->sizeData>>16)& 0xFF; // On décalle de 16 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[4]=(dataTrame->sizeData>>8)& 0xFF; // On décalle de 8 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[5]=(dataTrame->sizeData)& 0xFF;
    return trame;
}

PDataTrame decodeDataHead(char * data, int dataFd){
    PDataTrame trameDecod = (PDataTrame) malloc(sizeof(DataTrame));
    trameDecod->cmd=data[0];
    trameDecod->status=data[1];
    trameDecod->sizeData=0;
    trameDecod->sizeData|=(data[2]& 0xFF)<<24;
    trameDecod->sizeData|=(data[2]& 0xFF)<<16;
    trameDecod->sizeData|=(data[2]& 0xFF)<<8;
    trameDecod->sizeData|=(data[2]& 0xFF);
    trameDecod->dataFd=dataFd;
    // on check tous les problèmes potentiel pouvant subevenir
    if (checkDataTrameError(trameDecod)==true){
        return (char *) (0xFF);
    }
    return trameDecod;
}
