#include "all-test.h"
#include "test-core.h"
#include "utils.h"

//Pour le cas d'un fichier mp4
bool checkTypeMp4_itShouldPass_1() {
    char* type = getType("tests/types/Eno:r;m.e.mp4");
    int different = strcmp(type, "video/mp4");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier gif
bool checkTypeGif_itShouldPass_2() {
    char* type = getType("tests/types/1584641959_264125101_0.gif");
    int different = strcmp(type, "image/gif");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier bat
bool checkTypeBat_itShouldPass_3() {
    char* type = getType("tests/types/start.bat");
    int different = strcmp(type, "text/plain");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier ico
bool checkTypeIco_itShouldPass_4() {
    char* type = getType("tests/types/t430_v4.ico");
    int different = strcmp(type, "image/vnd.microsoft.icon");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier docx
bool checkTypeDocx_itShouldPass_5() {
    char* type = getType("tests/types/dim_perso.docx");
    int different = strcmp(type, "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier qui n'existe pas
bool checkTypeNonExistent_itShouldPass_6() {
    char* type = getType("tests/types/unfound.csv");
    int different = strcmp(type, "unfound_error");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

//Pour le cas d'un fichier avec un ou plusieurs caractères ' ' dans son nom
bool checkTypeWithSpace_itShouldPass_7() {
    char* type = getType("tests/types/github-git cheat sheet.pdf");
    int different = strcmp(type, "space_error");
    if (different)
    {
        printf("Le test échoue car vous n'êtes pas à la racine du projet\n");
    }
    return (different==0);
}

void testType() {
    printTitle("Test de la classe type");
    printTitle("Test de la fonction getType");
    passTest("getType","it should return 'video/mp4'", checkTypeMp4_itShouldPass_1());
    passTest("getType","it should return 'image/gif'", checkTypeGif_itShouldPass_2());
    passTest("getType","it should return 'text/plain'", checkTypeBat_itShouldPass_3());
    passTest("getType","it should return 'image/vnd.microsoft.icon'", checkTypeIco_itShouldPass_4());
    passTest("getType","it should return 'application/vnd.openxmlformats-officedocument.wordprocessingml.document'", checkTypeDocx_itShouldPass_5());
    passTest("getType","it should return 'unfound_error'", checkTypeNonExistent_itShouldPass_6());
    passTest("getType","it should return 'space_error'", checkTypeWithSpace_itShouldPass_7());
}