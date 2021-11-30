#include "test-core.h"

int testPassed = 0;
int totalTest = 0;

void print_hex(const unsigned char *s)
{
    //tant que le pointeur n'est pas à la fin de la chaine
    while(*s)
        //on affiche l'octet en hexa
        printf("%02x-", (unsigned int) *s++);
    printf("\n");
}

void printTitle(char *title) {
    //on encadre le titre de tirets grace à la taille du string
    unsigned int size = strlen(title);
    printf(COLOR_BLUE"\n+");
    for (int i = 0; i < size + 4; i++) {
        printf("-");
    }
    printf("+\n|"COLOR_BLUE COLOR_YELLOW"  %s  "COLOR_BLUE COLOR_BLUE"|\n+", title);
    for (int i = 0; i < size + 4; i++) {
        printf("-");
    }
    printf("+\n"COLOR_RESET);
}

void passTest(char *testFunction, char *msg, bool res) {
    //afiche si le test est passé et incrémente le nombre de tests validés
    totalTest++;
    printf(COLOR_BLUE"%s : %s\n"COLOR_RESET, testFunction, msg);
    if (res) {
        printf(COLOR_GREEN"\t✔️ test passed\n"COLOR_RESET);
        testPassed++;
    } else {
        printf(COLOR_RED"\t❌ test failure\n"COLOR_RESET);
    }
}

void resume() {
    //on affiche le nombre de tests validés et échoués
    printf(COLOR_YELLOW"\n%d test réussi sur %d (%d/%d)\n"COLOR_RESET, testPassed, totalTest, testPassed, totalTest);
}
