#include "type.h"

/**
 * getType utilise un exec sur la commande File 
 * et lit dans l'entrée standard pour récupérer 
 * le type de fichier
*/
char* getType(char * filePath) {
    int pipeFile[2];
    pipe(pipeFile);
    switch(fork()){
        case -1:
            perror("Fork error");
            exit(-1);
        //premier process fis
        case 0:
            //Fermer l'entrée du pipe
            close(pipeFile[0]);

            //Femer stdout
            close(1);
            //on clone le pipe dans le stdout
            dup(pipeFile[1]);
            close(pipeFile[1]);
            if(execlp("file","file","-i", filePath,NULL)!=-1){
                perror("Fork error");
                exit(-1);
            }
        default:
            wait(NULL);
    }

    char tampon[255];
    read(pipeFile[0], tampon, 255);
    close(pipeFile[0]);
    close(pipeFile[1]);

    const char s1[2] = " ";
    const char s2[2] = ";";
    char *token = malloc(sizeof(char)*255);
   
    /* get the first token (before the first ' ') */
    token = strtok(tampon, s1);
   
    /* go to the token that we want (after the first ' ' and before the ';') */
    token = strtok(NULL, s2);
    char* check1 = strstr(token, ":");      //if a file has a space in his name, we will get ':' in our token
    char* check2 = strstr(token,"cannot open");     //if we search a file that doesn't exist or if we're not on the project's root
    if (check1!=NULL && check2 == NULL) {
        //There's a problem with the name of the file, due to some ' ' characters in it
        token = "space_error";
    } else if (check2 != NULL) {
        token = "unfound_error";
    }
    return token;
}