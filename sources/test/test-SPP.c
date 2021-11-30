#include "test-core.h"
#include "spp.h"

//Test test SPP
/*----------------Debut Test de la fonction encodeDataHead-----------------*/

// Pour le cas de EMPTY_CMD
bool encodeDataHead_itShouldReturnCmdEMPTYCMDError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_CMD) && (dataTrame[0] == 0xff);
}

// Pour le cas de CMD_ERROR
bool encodeDataHead_itShouldRetunrCMDERRORError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xF0;
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == CMD_ERROR) && (dataTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool encodeDataHead_itShouldReturnEMPTYSTATUSError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_STATUS) && (dataTrame[0] == 0xff);
}

// Pour le cas de EMPTY_FD
bool encodeDataHead_itShouldReturnEMPTYFDError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_FD) && (dataTrame[0] == 0xff);
}

// Pour le cas de FD_ERROR
bool encodeDataHead_itShouldReturnFDERRORError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = -1;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == FD_ERROR) && (dataTrame[0] == 0xff);
}

// Pour le cas de EMPTY_SIZE_DATA
bool encodeDataHead_itShouldReturnEMPTYSIZEDATAError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_SIZE_DATA) && (dataTrame[0] == 0xff);
}
/*----------------FIN Test de la fonction encodeDataHead-----------------*/


/*----------------Debut Test de la fonction decodeDataHead-----------------*/

// Pour le cas de EMPTY_CMD
bool decodeDataHead_itShouldReturnCmdEMPTYCMDError(){
    SPP_DATA_TRAME_Erno = -1;
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = decodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_CMD) && (dataTrame[0] == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeDataHead_itShouldReturnCMDERRORError(){
    SPP_DATA_TRAME_Erno = -1;
    data->cmd = 0xF0;
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = decodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == CMD_ERROR) && (dataTrame[0] == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeDataHead_itShouldReturnEMPTY_STATUSError(){
    SPP_DATA_TRAME_Erno = -1;
    data->cmd = 0xAD;
    data->status = 0;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = decodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_STATUS) && (dataTrame[0] == 0xff);

}
/*----------------FIN Test de la fonction decodeDataHead-----------------*/


/*----------------Debut Test de la fonction encodeInfosTrame-----------------*/

// Pour le cas de EMPTY_CMD
bool encodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_INFOS_TRAME_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->status = SUCCESS;
    infos->nbFiles = 0;
    infos->sizeInfos=0;
    char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (SPP_INFOS_TRAME_Erno == EMPTY_CMD) && (infosTrame[0] == 0xff);
}

// Pour le cas de CMD_ERROR
bool encodeInfosTrame_itShouldRetunrCMDERRORError(){
    SPP_INFOS_TRAME_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = SUCCESS;
    infos->sizeData = 0;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_INFOS_TRAME_Erno == CMD_ERROR) && (infosTrame[0] == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool encodeInfosTrame_itShouldRetunrEMPTYSTATUSError(){
    SPP_INFOS_TRAME_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = '';
    infos->sizeData = 0;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_INFOS_TRAME_Erno == EMPTY_STATUS) && (infosTrame[0] == 0xff);
}

// Pour le cas de WRONG_SIZE
bool encodeInfosTrame_itShouldRetunrWRONGSIZEError(){
    SPP_INFOS_TRAME_Erno = -1;
    PInfosTrame infos = (PInfosTrame) malloc(sizeof(PInfosTrame));
    infos->cmd = 0xF0;
    infos->status = '';
    infos->sizeData = -1;
    char* infosTrame = char* encodeInfosTrame(infos);
    free(infos);
    return (SPP_INFOS_TRAME_Erno == WRONG_SIZE) && (infosTrame[0] == 0xff);
}

/*----------------FIN Test de la fonction encodeInfosTrame-----------------*/

/*----------------Debut Test de la fonction decodeInfosTrame-----------------*/
// Pour le cas de EMPTY_CMD
bool decodeInfosTrame_itShouldReturnCmdEMPTYCMDError(){
    SPP_DATA_TRAME_Erno = -1;
    char * trame={0,SUCCESS,0,0,0}
    infosTrame dataTrame = decodeInfosTrame(trame,5);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_CMD) && (dataTrame->cmd == 0xff);
}

// Pour le cas de CMD_ERROR
bool decodeInfosTrame_itShouldReturnCMDERRORError(){
    SPP_DATA_TRAME_Erno = -1;
    data->cmd = 0xF0;
    data->status = SUCCESS;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = decodeInfosTrame(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == CMD_ERROR) && (dataTrame[0] == 0xff);
}
// Pour le cas de EMPTY_STATUS
bool decodeInfosTrame_itShouldReturnEMPTY_STATUSError(){
    SPP_DATA_TRAME_Erno = -1;
    data->cmd = 0xAD;
    data->status = 0;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = decodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == EMPTY_STATUS) && (dataTrame[0] == 0xff);

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

    passTest("decodeDataHead","it should return EMPTY_CMD",decodeDataHead_itShouldReturnCmdEMPTYCMDError());
    passTest("decodeDataHead","it should return CMD_ERROR",decodeDataHead_itShouldReturnCMDERRORError());
    passTest("decodeDataHead","it should return EMPTY_STATUS",decodeDataHead_itShouldReturnEMPTY_STATUSError());

    passTest("encodeInfosTrame","it should return EMPTY_CMD",encodeInfosTrame_itShouldReturnCmdEMPTYCMDError());
    passTest("encodeInfosTrame","it should return CMD_ERROR",encodeInfosTrame_itShouldRetunrCMDERRORError());
    passTest("encodeInfosTrame","it should return EMPTY_STATUS",encodeInfosTrame_itShouldRetunrEMPTYSTATUSError());
    passTest("encodeInfosTrame","it should return WRONG_SIZE",encodeInfosTrame_itShouldRetunrWRONGSIZEError());



    printTitle("Test de la fonction decodeDataHead");

}

