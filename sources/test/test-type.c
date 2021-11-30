#include "all-test.h"
#include "test-core.h"
#include "type.h"

// Pour le cas de EMPTY_CMD 
// bool encodeDataHead_itShouldReturnCmdEMPTYCMDError(){     
//     SPP_DATA_TRAME_Erno = -1;     
//     PDataTrame data = (PDataTrame) malloc(sizeof(DataTrame));
//     data->status = SUCCESS;     
//     data->sizeData = 0;     
//     data->dataFd = 0;     
//     char* dataTrame = encodeDataHead(data);     
//     free(data);     
//     return (SPP_DATA_TRAME_Erno == EMPTY_CMD) && (dataTrame[0] == 0xff); 
// }

//Pour le cas d'un fichier mp4
bool checkTypeMp4_itShouldPass_1() {
    char* type = getType("sources/test/types/Enorme.mp4");
    return (strcmp(type,"video/mp4")==0);
}


void testType() {
    printTitle("Test de la classe type");
    printTitle("Test de la fonction getType");
    passTest("getType","it shoulds return 'video/mp4'", checkTypeMp4_itShouldPass_1());
}