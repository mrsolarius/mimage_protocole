#include "test-core.h"
#include "spp.h"
#include <stdbool.h>

//Test test SPP
/*----------------Debut Test de la fonction encodeDataHead-----------------*/

// Pour le cas de EMPTY_CMD
bool encodeDataHead_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == EMPTY_CMD) && (dataTrame[0] == 0xff);
}

// Pour le cas de CMD_ERROR
bool encodeDataHead_itShouldRetunrCMDERRORError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xF0;
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == CMD_ERROR) && (dataTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool encodeDataHead_itShouldReturnEMPTYSTATUSError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == EMPTY_STATUS) && (dataTrame[1] == 0xff);
}

// Pour le cas de EMPTY_FD
bool encodeDataHead_itShouldReturnEMPTYFDError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == EMPTY_FD) && (dataTrame[3] == 0xff);
}

// Pour le cas de FD_ERROR
bool encodeDataHead_itShouldReturnFDERRORError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = -1;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == FD_ERROR) && (dataTrame[3] == 0xff);
}

// Pour le cas de EMPTY_SIZE_DATA
bool encodeDataHead_itShouldReturnEMPTYSIZEDATAError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_Erno == EMPTY_SIZE_DATA) && (dataTrame[2] == 0xff);
}
/*----------------FIN Test de la fonction encodeDataHead-----------------*/

/*Partie des test commanté car à revoir (structure envoyer à la place d'un char *)*/
/*----------------Debut Test de la fonction decodeDataHead-----------------*/

// Pour le cas de EMPTY_CMD
bool decodeDataHead_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    char * trame={0,SUCCESS,0,0}
    PDataTrame dataTrame = decodeDataHead(trame);
    free(datatrame);
    return (SPP_Erno == EMPTY_CMD) && (dataTrame->cmd == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeDataHead_itShouldReturnCMDERRORError(){
    SPP_Erno = -1;
    char * trame={0xF0,SUCCESS,0,0}
    PDataTrame dataTrame = decodeDataHead(trame);
    free(dataTrame);
    return (SPP_Erno == CMD_ERROR) && (dataTrame->cmd == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeDataHead_itShouldReturnEMPTY_STATUSError(){
    SPP_Erno = -1;
    char * trame={0xAD,0,0,0}
    PDataTrame dataTrame = decodeDataHead(trame);
    free(dataTrame);
    return (SPP_Erno == EMPTY_STATUS) && (dataTrame->status == 0xff);

}
/*----------------FIN Test de la fonction decodeDataHead-----------------*/

/*----------------Debut Test de la fonction checkInfoTrameError-----------------*/
// Pour le cas de CMD_ERROR
bool checkInfoTrameError_itShouldRetunrCMDERRORError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeData = 0;
    bool infosTrame = bool checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == CMD_ERROR) && (infosTrame[0] == 0xff);
}

bool checkInfoTrameError_itShouldRetunrEMPTYCMDError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->status = SUCCESS;
    infos->nbFiles = 0;
    infos->sizeInfos=0;
    bool infosTrame = bool checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == EMPTY_CMD) && (infosTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool checkInfoTrameError_itShouldRetunrEMPTYSTATUSError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = '';
    infos->sizeData = 0;
    bool infosTrame = bool checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame[1] == 0xff);
}

// Pour le cas de WRONG_SIZE
bool checkInfoTrameError_itShouldRetunrWRONGSIZEError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = 0;
    infos->sizeData = -1;
    bool infosTrame = bool checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == WRONG_SIZE) && (infosTrame[3] == 0xff);
}

/*----------------FIN Test de la fonction checkInfoTrameError-----------------*/

/*----------------Debut Test de la fonction encodeInfosTrame-----------------*/

// Pour le cas de EMPTY_CMD
bool encodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->status = SUCCESS;
    infos->nbFiles = 0;
    infos->sizeInfos=0;
    char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == EMPTY_CMD) && (infosTrame[0] == 0xff);
}

// Pour le cas de CMD_ERROR
bool encodeInfosTrame_itShouldRetunrCMDERRORError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeData = 0;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == CMD_ERROR) && (infosTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool encodeInfosTrame_itShouldRetunrEMPTYSTATUSError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = '';
    infos->sizeData = 0;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame[0] == 0xff);
}

// Pour le cas de WRONG_SIZE
bool encodeInfosTrame_itShouldRetunrWRONGSIZEError(){
    SPP_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = '';
    infos->sizeData = -1;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == WRONG_SIZE) && (infosTrame[0] == 0xff);
}

/*----------------FIN Test de la fonction encodeInfosTrame-----------------*/

/*----------------Debut Test de la fonction decodeInfosTrame-----------------*/
// Pour le cas de EMPTY_CMD
bool decodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    char * trame={0,SUCCESS,0,0,0}
    infosTrame dataTrame = decodeInfosTrame(trame,5);
    free(data);
    return (SPP_Erno == EMPTY_CMD) && (dataTrame->cmd == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeInfosTrame_itShouldReturnCMDERRORError(){
    SPP_Erno = -1;
    char * trame={0xF0,SUCCESS,0,0,0};
    InfosTrame infosTrame = decodeInfosTrame(trame,5);
    free(infosTrame);
    return (SPP_Erno == CMD_ERROR) && (infosTrame->cmd == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeInfosTrame_itShouldReturnEMPTYSTATUSError(){
    SPP_Erno = -1;
    char * trame={0xAD,0,0,0,0};
    InfosTrame infosTrame = decodeInfosTrame(trame,5);
    free(infosTrame);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame->status == 0xff);

}

// Pour le cas de EMPTY_STATUS
bool decodeInfosTrame_itShouldReturnWRONGSIZEError(){
    SPP_Erno = -1;
    char * trame={0xF0,0,0,-1,0};
    InfosTrame infosTrame = decodeInfosTrame(trame,5);
    free(infosTrame);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame->status == 0xff);

}

/*----------------FIN Test de la fonction decodeInfosTrame-----------------*/

//Fonction d'appel principal de SPP
void testSPP(){
    printTitle("Test de la classe SPP");

    printTitle("Test de la fonction encodeDataHead");
    passTest("encodeDataHead","it should return cmd empty error",encodeDataHead_itShouldReturnCmdEMPTYCMDError());
    passTest("encodeDataHead","it should return cmd error",encodeDataHead_itShouldRetunrCMDERRORError());
    passTest("encodeDataHead","it should return status empty error",encodeDataHead_itShouldReturnEMPTYSTATUSError());
    passTest("encodeDataHead","it should return EMPTYFD",encodeDataHead_itShouldReturnEMPTYFDError());
    passTest("encodeDataHead","it should return FDERROR",encodeDataHead_itShouldReturnFDERRORError());
    passTest("encodeDataHead","it should return EMPTY_SIZE_DATA",encodeDataHead_itShouldReturnEMPTYSIZEDATAError());

    printTitle("Test de la fonction decodeDataHead");
    passTest("decodeDataHead","it should return EMPTY_CMD",decodeDataHead_itShouldReturnCmdEMPTYCMDError());
    passTest("decodeDataHead","it should return CMD_ERROR",decodeDataHead_itShouldReturnCMDERRORError());
    passTest("decodeDataHead","it should return EMPTY_STATUS",decodeDataHead_itShouldReturnEMPTY_STATUSError());

    printTitle("Test de la fonction encodeInfosTrame");
    passTest("encodeInfosTrame","it should return EMPTY_CMD",encodeInfosTrame_itShouldReturnCmdEMPTYCMDError());
    passTest("encodeInfosTrame","it should return CMD_ERROR",encodeInfosTrame_itShouldRetunrCMDERRORError());
    passTest("encodeInfosTrame","it should return EMPTY_STATUS",encodeInfosTrame_itShouldRetunrEMPTYSTATUSError());
    passTest("encodeInfosTrame","it should return WRONG_SIZE",encodeInfosTrame_itShouldRetunrWRONGSIZEError());
    
    printTitle("Test de la fonction decodeInfosTrame");
    passTest("decodeInfosTrame","it should return EMPTY_CMD",encodeInfosTrame_itShouldReturnCmdEMPTYCMDError());
    passTest("decodeInfosTrame","it should return CMD_ERROR",decodeInfosTrame_itShouldReturnCMDERRORError());
    passTest("decodeInfosTrame","it should return EMPTY_STATUS",decodeInfosTrame_itShouldReturnEMPTYSTATUSError());
    passTest("decodeInfosTrame","it should return WRONG_SIZE",decodeInfosTrame_itShouldReturnWRONGSIZEError());
    
    printTitle("Test de la fonction checkInfoTrameError");
    passTest("checkInfoTrameError","it should return EMPTY_CMD",checkInfoTrameError_itShouldRetunrEMPTYCMDError());
    passTest("checkInfoTrameError","it should return CMD_ERROR",checkInfoTrameError_itShouldRetunrCMDERRORError());
    passTest("checkInfoTrameError","it should return EMPTY_STATUS",checkInfoTrameError_itShouldRetunrEMPTYSTATUSError());
    passTest("checkInfoTrameError","it should return WRONG_SIZE",checkInfoTrameError_itShouldRetunrWRONGSIZEError());
    
}

