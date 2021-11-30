#include "test-core.h"
#include "spp.h"

//Test test SPP
//Test de la fonction encodeDataHead
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

bool encodeDataHead_itShouldReturnFDERROR(){
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

//Test de la fonction decodeDataHead
bool decodeDataHead_itShouldReturnCmdError(){
    return true;
}

//Fonction d'appel principal de SPP
void testSPP(){
    printTitle("Test de la classe SPP");
    printTitle("Test de la fonction encodeDataHead");
    passTest("encodeDataHead","it should return cmd empty error",encodeDataHead_itShouldReturnCmdEMPTYCMDError());
    passTest("encodeDataHead","it should return cmd error",encodeDataHead_itShouldRetunrCMDERRORError());
    passTest("encodeDataHead","it should return status empty error",encodeDataHead_itShouldReturnEMPTYSTATUSError());
    passTest("encodeDataHead","it should return EMPTYFD",encodeDataHead_itShouldReturnEMPTYFDError());
    passTest("encodeDataHead","it should return FDERROR",encodeDataHead_itShouldReturnFDERROR());

    printTitle("Test de la fonction decodeDataHead");

}

