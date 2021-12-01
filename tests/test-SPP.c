#include "test-core.h"
#include "spp.h"
#include <stdbool.h>

//Test test SPP
/*----------------Début de test de la fonction checkDataTrameError----------*/
bool checkDataTrameError_ShouldReturnEMPTY_CMD(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->status = SUCCESS;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == EMPTY_CMD) && (res == true);
}

bool checkDataTrameError_ShouldReturnCMD_ERROR(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=ACQUIT_FILE_DATA;
    data->status = SUCCESS;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == CMD_ERROR) && (res == true); 
}

bool checkDataTrameError_ShouldReturnCMD_ERROR_1(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=0xFD;
    data->status = SUCCESS;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == CMD_ERROR) && (res == true); 
}

bool checkDataTrameError_ShouldReturnEMPTY_STATUS(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == EMPTY_STATUS) && (res == true);
}

bool checkDataTrameError_ShouldReturnSTATUS_ERROR(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->status = 0xFE;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == STATUS_ERROR) && (res == true);
}

bool checkDataTrameError_ShouldReturnEMPTY_FD(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->status = SUCCESS;
    data->sizeData = 256;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == EMPTY_FD) && (res == true);
}

bool checkDataTrameError_ShouldReturnEMPTY_SIZE_DATA(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->status = SUCCESS;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == EMPTY_SIZE_DATA) && (res == true);
}

bool checkDataTrameError_ShouldPassWithDATA(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->status = SUCCESS;
    data->sizeData = 256;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == -1) && (res == false);
}

bool checkDataTrameError_ShouldPassWithError(){
    SPP_Erno=-1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=UPLOAD_FILE_DATA;
    data->status = NO_FOUND_FILE;
    data->sizeData = 0;
    data->dataFd = 1;
    bool res = checkDataTrameError(data);
    return (SPP_Erno == -1) && (res == false);
    
}
/*----------------Debut Test de la fonction encodeDataHead-----------------*/

// Pour le cas d'envoie de data
bool encodeDataHead_itShouldReturnCorrectFrame(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = DOWNLOAD_FILE_DATA;
    data->status = SUCCESS;
    data->sizeData = 2320441089;
    data->dataFd = 1;
    unsigned char* dataTrame = encodeDataHead(data);
    free(data);
    return (
        (dataTrame[0]==DOWNLOAD_FILE_DATA)&&
        (dataTrame[1]==SUCCESS)&&
        (dataTrame[2]==0b10001010)&&
        (dataTrame[3]==0b01001111)&&
        (dataTrame[4]==0b00011111)&&
        (dataTrame[5]==0b00000001)
    );
}

// pour le cas d'envoie d'erreur
bool encodeDataHead_itShouldReturnCorrectFrameWithStatusError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = DOWNLOAD_FILE_DATA;
    data->status = NO_FOUND_FILE;
    data->dataFd = 1;
    unsigned char* dataTrame = encodeDataHead(data);
    free(data);
    return (
        (dataTrame[0]==DOWNLOAD_FILE_DATA)&&
        (dataTrame[1]==NO_FOUND_FILE)&&
        (dataTrame[2]==0b0)&&
        (dataTrame[3]==0b0)&&
        (dataTrame[4]==0b0)&&
        (dataTrame[5]==0b0)
    );
}
/*----------------FIN Test de la fonction encodeDataHead-----------------*/

/*Partie des test commanté car à revoir (structure envoyer à la place d'un char *)*/
/*----------------Debut Test de la fonction decodeDataHead-----------------*/

// Pour le cas de EMPTY_CMD
bool decodeDataHead_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    char trame[]={0,SUCCESS,0,0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    free(dataTrame);
    return (SPP_Erno == EMPTY_CMD) && (dataTrame->cmd == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeDataHead_itShouldReturnCMDERRORError(){
    SPP_Erno = -1;
    char trame[]={0xF0,SUCCESS,0,0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    free(dataTrame);
    return (SPP_Erno == CMD_ERROR) && (dataTrame->cmd == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeDataHead_itShouldReturnEMPTY_STATUSError(){
    SPP_Erno = -1;
    char trame[]={0xAD,0,0,0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    free(dataTrame);
    return (SPP_Erno == EMPTY_STATUS) && (dataTrame->status == 0xff);

}
/*----------------FIN Test de la fonction decodeDataHead-----------------*/

/*----------------Debut Test de la fonction checkInfoTrameError-----------------*/
// Pour le cas de CMD_ERROR
bool checkInfoTrameError_itShouldRetunrCMDERRORError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == CMD_ERROR) && (infosTrame == true);
}

bool checkInfoTrameError_itShouldRetunrEMPTYCMDError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->status = SUCCESS;
    infos->nbFiles = 0;
    infos->sizeInfos=0;
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == EMPTY_CMD) && (infosTrame==true);
}

// Pour le cas de EMPTY_STATUS
bool checkInfoTrameError_itShouldRetunrEMPTYSTATUSError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = LIST_SIZE;
    infos->sizeInfos = 0;
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame==true);
}

// Pour le cas de STATUS_ERROR
bool checkInfoTrameError_itShouldReturnSTATUSERROR(){
    SPP_Erno=-1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = LIST_SIZE;
    infos->status = 0xFE;
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == STATUS_ERROR) && (infosTrame == true);
}

// Pour le cas de WRONG_SIZE
bool checkInfoTrameError_itShouldRetunrWRONGSIZEError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = LIST_SIZE;
    infos->status = SUCCESS;
    infos->sizeInfos = 20;
    infos->infos = "azerty";
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == WRONG_SIZE) && (infosTrame==true);
}

/*----------------FIN Test de la fonction checkInfoTrameError-----------------*/

/*----------------Debut Test de la fonction encodeInfosTrame-----------------*/

// Pour le cas de EMPTY_CMD
bool encodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
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
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    char* infosTrame =encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == CMD_ERROR) && (infosTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool encodeInfosTrame_itShouldRetunrEMPTYSTATUSError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xF0;
    infos->sizeInfos = 0;
    char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == EMPTY_STATUS) && (infosTrame[1] == 0xff);
}

// Pour le cas de WRONG_SIZE
bool encodeInfosTrame_itShouldRetunrWRONGSIZEError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeInfos = -1;
    char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (SPP_Erno == WRONG_SIZE) && (infosTrame[0] == 0xff);
}

/*----------------FIN Test de la fonction encodeInfosTrame-----------------*/

/*----------------Debut Test de la fonction decodeInfosTrame-----------------*/
// Pour le cas de EMPTY_CMD
bool decodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_Erno = -1;
    char trame[]={0,SUCCESS,0,0,0};
    PInfoTrame infosTrame = decodeInfosTrame(trame,5);
    return (SPP_Erno == EMPTY_CMD) && (infosTrame->cmd == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeInfosTrame_itShouldReturnCMDERRORError(){
    SPP_Erno = -1;
    char trame[]={0xF0,SUCCESS,0,0,0};
    PInfoTrame infosTrame = decodeInfosTrame(trame,5);

    return (SPP_Erno == CMD_ERROR) && (infosTrame->cmd == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeInfosTrame_itShouldReturnEMPTYSTATUSError(){
    SPP_Erno = -1;
    char trame[]={0xAD,0,0,0,0};
    PInfoTrame infosTrame = decodeInfosTrame(trame,5);

    return (SPP_Erno == EMPTY_STATUS) && (infosTrame->status == 0xff);

}

// Pour le cas de EMPTY_STATUS
bool decodeInfosTrame_itShouldReturnWRONGSIZEError(){
    SPP_Erno = -1;
    char trame[]={0xF0,0,0,-1,0};
    PInfoTrame infosTrame = decodeInfosTrame(trame,5);

    return (SPP_Erno == EMPTY_STATUS) && (infosTrame->status == 0xff);

}

/*----------------FIN Test de la fonction decodeInfosTrame-----------------*/

//Fonction d'appel principal de SPP
void testSPP(){
    printTitle("Test de la classe SPP");
    printTitle("Test de la fonction checkDataTrameError");
    passTest("checkDataTrameError","it should return EMPTY_CMD",checkDataTrameError_ShouldReturnEMPTY_CMD());
    passTest("checkDataTrameError","it should return CMD_ERROR",checkDataTrameError_ShouldReturnCMD_ERROR());
    passTest("checkDataTrameError","it should return CMD_ERROR",checkDataTrameError_ShouldReturnCMD_ERROR_1());
    passTest("checkDataTrameError","it should return EMPTY_STATUS",checkDataTrameError_ShouldReturnEMPTY_STATUS());
    passTest("checkDataTrameError","it should return STATUS_ERROR",checkDataTrameError_ShouldReturnSTATUS_ERROR());
    passTest("checkDataTrameError","it should return EMPTY_SIZE_DATA",checkDataTrameError_ShouldReturnEMPTY_SIZE_DATA());
    passTest("checkDataTrameError","it should pass test with data_size",checkDataTrameError_ShouldPassWithDATA());
    passTest("checkDataTrameError","it should pass test without data_size",checkDataTrameError_ShouldPassWithError());

    printTitle("Test de la fonction encodeDataHead");
    /*passTest("encodeDataHead","it should return cmd empty error",encodeDataHead_itShouldReturnCmdEMPTYCMDError());
    passTest("encodeDataHead","it should return cmd error",encodeDataHead_itShouldRetunrCMDERRORError());
    passTest("encodeDataHead","it should return status empty error",encodeDataHead_itShouldReturnEMPTYSTATUSError());
    passTest("encodeDataHead","it should return EMPTYFD",encodeDataHead_itShouldReturnEMPTYFDError());
    passTest("encodeDataHead","it should return FDERROR",encodeDataHead_itShouldReturnFDERRORError());*/
    passTest("encodeDataHead","it should return Correct Frame",encodeDataHead_itShouldReturnCorrectFrame());
    passTest("encodeDataHead","it should return Correct Frame with status Error",encodeDataHead_itShouldReturnCorrectFrameWithStatusError());

    /*
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
    passTest("checkInfoTrameError","it should return STATUS_ERROR",checkInfoTrameError_itShouldReturnSTATUSERROR());
    passTest("checkInfoTrameError","it should return WRONG_SIZE",checkInfoTrameError_itShouldRetunrWRONGSIZEError());
    
}

