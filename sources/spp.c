#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "spp.h"

SPP_Erno=-1;

bool checkInfoTrameError(PInfoTrame infosTrame){
    bool test=false;
    if (infosTrame->cmd==0){
        SPP_Erno=EMPTY_CMD;
        test=true;
    }
    if ((infosTrame->cmd==DOWNLOAD_FILE_NAME)||(infosTrame->cmd==UPLOAD_FILE_DATA)){
        SPP_Erno=CMD_ERROR;
        test=true;
    }
    if (infosTrame->status==0){
        SPP_Erno=EMPTY_STATUS;
        test=true;
    }
    if ((infosTrame->sizeInfos!=strlen(infosTrame->infos))){
        SPP_Erno=WRONG_SIZE;
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

