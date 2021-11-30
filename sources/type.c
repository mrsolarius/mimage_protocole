#include <stdio.h>


/**
 * getType utilise un exec sur la commande File 
 * et lit dans l'entrée standard pour récupérer 
 * le type de fichier
*/
char* getType(char * filePath) {
    int f[2];
    pipe(f);
    switch (fork()) {
        case -1 :
            perror("fork");
            exit(-1);
        case 0 :
            close(1);
            dup(f[1]);
            close(f[1]);
    }
}