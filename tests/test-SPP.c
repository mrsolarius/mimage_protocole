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
    data->sizeData = 0;
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

bool encodeDataHead_itShouldReturnThrowError(){
    SPP_Erno = -1;
    PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
    data->cmd=0xFD;
    data->status = NO_FOUND_FILE;
    data->dataFd = 1;
    unsigned char* dataTrame = encodeDataHead(data);
    free(data);
    return ((dataTrame[0]==0xff)&&(SPP_Erno==CMD_ERROR));
}
/*----------------FIN Test de la fonction encodeDataHead-----------------*/

/*Partie des test commanté car à revoir (structure envoyer à la place d'un char *)*/
/*----------------Debut Test de la fonction decodeDataHead-----------------*/

// Pour le cas de CMD_ERROR
bool decodeDataHead_itShouldReturnCmdERORR(){
    SPP_Erno = -1;
    unsigned char trame[]={0xFD,NO_FOUND_FILE,0b0,0b0,0b0,0b0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    return (SPP_Erno == CMD_ERROR) && (dataTrame->cmd == 0xff);
}

// Pour le cas de EMPTY_STATUS
bool decodeDataHead_itShouldReturnEMPTY_STATUSError(){
    SPP_Erno = -1;
    unsigned char trame[]={DOWNLOAD_FILE_DATA,0xFD,0b0,0b0,0b0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    return (SPP_Erno == STATUS_ERROR) && (dataTrame->cmd == 0xff);

}

// Pour verifier que la trame est bien décoder
bool decodeDataHead_itShouldPassWithData(){
    SPP_Erno = -1;
    unsigned char trame[]={DOWNLOAD_FILE_DATA,SUCCESS,0b10001010,0b01001111,0b00011111,0b00000001};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    return (SPP_Erno == -1) && (
        (dataTrame->cmd == DOWNLOAD_FILE_DATA)&&
        (dataTrame->status == SUCCESS) &&
        (dataTrame->sizeData == 2320441089)&&
        (dataTrame->dataFd ==1)
    );
}

// Pour verifier que la trame est bien décoder
bool decodeDataHead_itShouldPassWithError(){
    SPP_Erno = -1;
    unsigned char trame[]={DOWNLOAD_FILE_DATA,INTERNAL_ERROR,0,0,0,0};
    PDataTrame dataTrame = decodeDataHead(trame,1);
    return (SPP_Erno == -1) && (
        (dataTrame->cmd == DOWNLOAD_FILE_DATA)&&
        (dataTrame->status == INTERNAL_ERROR) &&
        (dataTrame->sizeData == 0)&&
        (dataTrame->dataFd ==1)
    );

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
    infos->infos = "";
    bool infosTrame = checkInfoTrameError(infos);
    free(infos);
    return (SPP_Erno == WRONG_SIZE) && (infosTrame==true);
}

// /*----------------FIN Test de la fonction checkInfoTrameError-----------------*/

// /*----------------Debut Test de la fonction encodeInfosTrame-----------------*/
// Pour le cas d'envoie de data
bool encodeInfosTrame_itShouldReturnCorrectFrame(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = DOWNLOAD_FILE_NAME;
    infos->status = SUCCESS;
    infos->sizeInfos = 6;
    infos->nbFiles = 0;
    infos->infos = "azerty";
    unsigned char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (
        (infosTrame[0]==DOWNLOAD_FILE_NAME)&&
        (infosTrame[1]==SUCCESS)&&
        (infosTrame[2]==0)&&
        (infosTrame[3]==6)&&
        (infosTrame[4]=='a')&&
        (infosTrame[5]=='z')&&
        (infosTrame[6]=='e')&&
        (infosTrame[7]=='r')&&
        (infosTrame[8]=='t')&&
        (infosTrame[9]=='y')
    );
}

// pour le cas d'envoie d'erreur
bool encodeInfosTrame_itShouldReturnCorrectFrameWithoutInfo(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = GET_FILE_DATA;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return (
        (infosTrame[0]==GET_FILE_DATA)&&
        (infosTrame[1]==SUCCESS)&&
        (infosTrame[2]==0)&&
        (infosTrame[3]==0)
    );
}

bool encodeInfosTrame_itShouldReturnThrowError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xfd;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return ((infosTrame[0]==0xff)&&(SPP_Erno==CMD_ERROR));
}

/*----------------FIN Test de la fonction encodeInfosTrame-----------------*/

/*----------------Debut Test de la fonction decodeInfosTrame-----------------*/
// Pour le cas d'envoie de data
bool decodeInfosTrame_itShouldReturnCorrectFrame(){
    SPP_Erno = -1;
    unsigned char * infosTrame = malloc(sizeof(unsigned char)*10);
    infosTrame[0]=DOWNLOAD_FILE_NAME;
    infosTrame[1]=SUCCESS;
    infosTrame[2]=0;
    infosTrame[3]=6;
    infosTrame[4]='a';
    infosTrame[5]='z';
    infosTrame[6]='e';
    infosTrame[7]='r';
    infosTrame[8]='t';
    infosTrame[9]='y';
    PInfoTrame trame = decodeInfosTrame(infosTrame,10);
    free(infosTrame);
    return (
        (trame->cmd==DOWNLOAD_FILE_NAME)&&
        (trame->status==SUCCESS)&&
        (trame->nbFiles==0)&&
        (trame->sizeInfos==6)&&
        (strcmp(trame->infos,"azerty")==0)
    );
}

// pour le cas d'envoie d'erreur
bool decodeInfosTrame_itShouldReturnCorrectFrameWithoutInfo(){
    SPP_Erno = -1;
    unsigned char * infosTrame = malloc(sizeof(unsigned char)*4);
    infosTrame[0]=GET_FILE_DATA;
    infosTrame[1]=SUCCESS;
    infosTrame[2]=0;
    infosTrame[3]=0;
    PInfoTrame trame = decodeInfosTrame(infosTrame,4);
    free(infosTrame);
    return (
        (trame->cmd == GET_FILE_DATA)&&
        (trame->status == SUCCESS)&&
        (trame->sizeInfos == 0)&&
        (trame->nbFiles == 0)
    );
}

bool decodeInfosTrame_itShouldReturnThrowError(){
    SPP_Erno = -1;
    PInfoTrame infos = (PInfoTrame) malloc(sizeof(PInfoTrame));
    infos->cmd = 0xfd;
    infos->status = SUCCESS;
    infos->sizeInfos = 0;
    infos->nbFiles = 0;
    unsigned char* infosTrame = encodeInfosTrame(infos);
    free(infos);
    return ((infosTrame[0]==0xff)&&(SPP_Erno==CMD_ERROR));
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
    passTest("encodeDataHead","it should return Correct Frame",encodeDataHead_itShouldReturnCorrectFrame());
    passTest("encodeDataHead","it should return Correct Frame with status Error",encodeDataHead_itShouldReturnCorrectFrameWithStatusError());
    passTest("encodeDataHead","it should return Error",encodeDataHead_itShouldReturnThrowError());


    printTitle("Test de la fonction decodeDataHead");
    passTest("decodeDataHead","it should return CMD_ERROR",decodeDataHead_itShouldReturnCmdERORR());
    passTest("decodeDataHead","it should return EMPTY_STATUS",decodeDataHead_itShouldReturnEMPTY_STATUSError());;
    passTest("decodeDataHead","it should return Correct Frame With data",decodeDataHead_itShouldPassWithData());
    passTest("decodeDataHead","it should return Correct Error Frame",decodeDataHead_itShouldPassWithError());
        
    printTitle("Test de la fonction checkInfoTrameError");
    passTest("checkInfoTrameError","it should return EMPTY_CMD",checkInfoTrameError_itShouldRetunrEMPTYCMDError());
    passTest("checkInfoTrameError","it should return CMD_ERROR",checkInfoTrameError_itShouldRetunrCMDERRORError());
    passTest("checkInfoTrameError","it should return EMPTY_STATUS",checkInfoTrameError_itShouldRetunrEMPTYSTATUSError());
    passTest("checkInfoTrameError","it should return STATUS_ERROR",checkInfoTrameError_itShouldReturnSTATUSERROR());
    passTest("checkInfoTrameError","it should return WRONG_SIZE",checkInfoTrameError_itShouldRetunrWRONGSIZEError());
    
    printTitle("Test de la fonction encodeInfosTrame");
    passTest("encodeInfosTrame","it should return Correct Frame",encodeInfosTrame_itShouldReturnCorrectFrame());
    passTest("encodeInfosTrame","it should return Correct Frame without info",encodeInfosTrame_itShouldReturnCorrectFrameWithoutInfo());
    passTest("encodeInfosTrame","it should return Error CMD",encodeInfosTrame_itShouldReturnThrowError());
    
    printTitle("Test de la fonction decodeInfosTrame");
    passTest("decodeInfosTrame","it should return correct frame",decodeInfosTrame_itShouldReturnCorrectFrame());
    passTest("decodeInfosTrame","it should return correct frame without info",decodeInfosTrame_itShouldReturnCorrectFrameWithoutInfo());
    
}

