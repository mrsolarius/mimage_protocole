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
    }
    return test;
}

bool checkInfoTrameSizeError(PInfoTrame infosTrame){
    bool test=false;
    if (infosTrame->infos == NULL)
    {
        if (infosTrame->sizeInfos != 0)
        {
            SPP_Erno=WRONG_SIZE;
            test=true;
        }  
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
    }else
    if (!((dataTrame->cmd==DOWNLOAD_FILE_DATA)||(dataTrame->cmd==UPLOAD_FILE_DATA))){
        SPP_Erno=CMD_ERROR;
        test=true;
    }else
    if (dataTrame->status==0){
        SPP_Erno=EMPTY_STATUS;
        test=true;
    }else
    if (!((dataTrame->status==SUCCESS)||
        (dataTrame->status==INVALID_NAME_FILE)||
        (dataTrame->status==INVALID_EXTEND_FILE)||
        (dataTrame->status==NAME_ALREADY_TAKEN)||
        (dataTrame->status==NO_FOUND_FILE)||
        (dataTrame->status==CHECKSUM_ERROR)||
        (dataTrame->status==INTERNAL_ERROR)||
        (dataTrame->status==LACK_OF_SPACE)||
        (dataTrame->status==FILE_TOO_LARGE))){
        SPP_Erno=STATUS_ERROR;
        test=true;
    }else
    if (dataTrame->dataFd==0){
        SPP_Erno=EMPTY_FD;
        test=true;
    }else
    if (dataTrame->dataFd<0){
        SPP_Erno=FD_ERROR;
        test=true;
    }else
    if ((dataTrame->status==SUCCESS)&&(dataTrame->sizeData==0)){
        SPP_Erno=EMPTY_SIZE_DATA;
        test=true;
    }
    return test;
}

unsigned char* encodeInfosTrame(PInfoTrame infosTrame){
    unsigned char *error = malloc(sizeof(unsigned char));
    // on check tous les problèmes potentiel pouvant subevenir
    if (checkInfoTrameError(infosTrame)==true){
        error[0]=0xff;
        return error;
    }
    //on met en place la structure de la trame après l'avoir déclarée
    unsigned char * trame;
    if(infosTrame->sizeInfos>0){
        trame = malloc(sizeof(char)*4 + strlen(infosTrame->infos)+1);
    }else{
        trame = malloc(sizeof(char)*4);
    }
    trame[0]=infosTrame->cmd;
    trame[1]=infosTrame->status;
    trame[2]=infosTrame->nbFiles;
    trame[3]=infosTrame->sizeInfos;
    for (int comp=0; comp < (infosTrame->sizeInfos); comp++){
        trame[comp+4] = infosTrame->infos[comp];
    }
    return trame;
}

PInfoTrame decodeInfosTrame(unsigned char* infos){
    PInfoTrame trameDecod = (PInfoTrame) malloc(sizeof(InfosTrame));
    trameDecod->cmd=infos[0];
    trameDecod->status=infos[1];
    trameDecod->nbFiles=infos[2];
    trameDecod->sizeInfos=infos[3];
    // on check tous les problèmes potentiels pouvant survenir
    if (checkInfoTrameError(trameDecod)==true){
        trameDecod->cmd=0xFF; 
    }
    return trameDecod;
}

//fonction qui prend une structure en parametre qui remplit le champ info avec le tableau de char en 2eme param
void decodeInfosTrame_Infos(PInfoTrame infoTrame, unsigned char* infos, int size) {
    infoTrame->infos = malloc(sizeof(char)*(size));
    for (int comp=0; comp < size; comp++) {
        infoTrame->infos[comp] = infos[comp];
    }
    if (checkInfoTrameSizeError(infoTrame)==true){
        infoTrame->cmd=0xFF; 
    }
}

unsigned char* encodeDataHead(PDataTrame dataTrame){
    unsigned char *error = malloc(sizeof(unsigned char));
    // on check tous les problèmes potentiel pouvant subevenir
    if (checkDataTrameError(dataTrame)==true){
        error[0]=0xff;
        return error;
    }

    unsigned char * trame= malloc(sizeof(unsigned char)*6); //soit la taille de 2 char et d'un long
    trame[0]=dataTrame->cmd;
    trame[1]=dataTrame->status;
    trame[2]=(dataTrame->sizeData>>24)& 0xFF; // On décalle de 24 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[3]=(dataTrame->sizeData>>16)& 0xFF; // On décalle de 16 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[4]=(dataTrame->sizeData>>8)& 0xFF; // On décalle de 8 bytes puis on prend un masque de 4 bytes pour le faire correspondre à un char.
    trame[5]=(dataTrame->sizeData)& 0xFF;
    free(error);
    return trame;
}

PDataTrame decodeDataHead(unsigned char * data, int dataFd){
    PDataTrame trameDecod = (PDataTrame) malloc(sizeof(DataTrame));
    trameDecod->cmd=data[0];
    trameDecod->status=data[1];
    trameDecod->sizeData=0;
    trameDecod->sizeData|=(data[2]& 0xFF)<<24;
    trameDecod->sizeData|=(data[3]& 0xFF)<<16;
    trameDecod->sizeData|=(data[4]& 0xFF)<<8;
    trameDecod->sizeData|=(data[5]& 0xFF);
    trameDecod->dataFd=dataFd;
    // on check tous les problèmes potentiels pouvant survenir
    if (checkDataTrameError(trameDecod)==true){
        trameDecod->cmd=0xFF; 
    }
    return trameDecod;
}
