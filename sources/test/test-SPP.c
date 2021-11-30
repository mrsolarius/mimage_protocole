#include "test-core.h"
#include "data_trame.h"

//Test test SPP
//Test de la fonction encodeDataHead
bool encodeDataHead_itShouldReturnCmdEmptyError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == cmdVide) && (dataTrame[0] == 0xff);
}

bool encodeDataHead_itShouldRetunrCmdError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xF0;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == cmdErreur) && (dataTrame[0] == 0xff);
}

bool encodeDataHead_itShouldReturnStatusEmptyError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x00;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == statusVide) && (dataTrame[0] == 0xff);
}

bool encodeDataHead_itShouldReturnSizeDataVideError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == sizeDataVide) && (dataTrame[0] == 0xff);
}

bool encodeDataHead_itShouldReturnFdVideError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == fdVide) && (dataTrame[0] == 0xff);
}

bool encodeDataHead_itShouldReturnFdError(){
    SPP_DATA_TRAME_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd = 0xAD;
    data->status = 0x20;
    data->sizeData = 0;
    data->dataFd = -1;
    char* dataTrame = encodeDataHead(data);
    free(data);
    return (SPP_DATA_TRAME_Erno == fdError) && (dataTrame[0] == 0xff);
}

//Test de la fonction decodeDataHead
bool decodeDataHead_itShouldReturnCmdError(){
    
}


//Fonction d'appel principal de SPP
void testSPP(){
    printTitle("Test de la classe SPP");
    printTitle("Test de la fonction encodeDataHead");
    passTest("encodeDataHead","it should return cmd empty error",encodeDataHead_itShouldReturnCmdEmptyError());
    passTest("encodeDataHead","it should return cmd error",encodeDataHead_itShouldReturnCmdError());
    passTest("encodeDataHead","it should return status empty error",encodeDataHead_itShouldReturnStatusEmptyError());
    passTest("encodeDataHead","it should return size data vide error",encodeDataHead_itShouldReturnSizeDataVideError());
    passTest("encodeDataHead","it should return fd vide error",encodeDataHead_itShouldReturnFdVideError());

    printTitle("Test de la fonction decodeDataHead");

}

