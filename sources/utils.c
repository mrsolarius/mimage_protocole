#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
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
    //Lire dans le pipe
    read(pipeFile[0], tampon, 255);
    //Fermeture du pipe
    close(pipeFile[0]);
    close(pipeFile[1]);

    const char s1[2] = " ";
    const char s2[2] = ";";
    char *token = malloc(sizeof(char)*255);
   
    // récupération du premier token (avant le premier ' ')
    token = strtok(tampon, s1);
   
    // on vas au token qui nous intéresse (après le premier ' ' et avant le ';')
    token = strtok(NULL, s2);
    // si le fichier a un espace dans son nom on aura un ':' dans le token
    char* check1 = strstr(token, ":");
    // si le fichier n'existe pas ou si on n'est pas dans le dossier du projet
    char* check2 = strstr(token,"cannot open");
    if (check1!=NULL && check2 == NULL) {
        //Il y a un problème avec le nom du fichier, à cause de certains ' ' dans son nom
        token = "space_error";
    } else if (check2 != NULL) {
        token = "unfound_error";
    }
    return token;
}

int countFiles(char * path){
    DIR * dir;
    struct dirent * ent;
    int count = 0;
    //On ouvre le dossier
    if ((dir = opendir (path)) != NULL) {
        //On parcours le dossier
        while ((ent = readdir (dir)) != NULL) {
            //On compte le nombre de fichier si le fichier n'est pas un fichier de retour
            if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0){
                count++;
            }
        }
        //On ferme le dossier
        closedir (dir);
    }
    //On retourne le nombre de fichier
    return count;
}

char ** getFiles(char * path, int nbFiles){
    DIR * dir;
    struct dirent * ent;
    //On alloue la mémoire pour le tableau de fichier avec le nombre de fichier
    char ** files = (char **) malloc(sizeof(char *) * nbFiles);
    if ((dir = opendir (path)) != NULL) {
        //On parcours le dossier
        while ((ent = readdir (dir)) != NULL) {
            //On ajoute le fichier dans le tableau s'il n'est pas un fichier de retour
            if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0){
                //On alloue la mémoire pour la taille du nom du fichier
                //+1 pour le caractère '\0'
                files[nbFiles-1] = (char *) malloc(sizeof(char) * (strlen(ent->d_name)+1));
                //On copie le nom du fichier dans le tableau
                strcpy(files[nbFiles-1],ent->d_name);
                //On décrémente le compteur pour passer à la case du tableau suivante
                nbFiles--;
            }
        }
        //On ferme le dossier
        closedir (dir);
    }
    //on renvoie le tableau tout allouer
    return files;
}

// fonction qui retourne le nombre de lignes d'un fichier et qui retourne le char* de chaque ligne
char ** fileToArray(char * filePath, int * nbLines){
    // On ouvre le fichier
    FILE * file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    // get the size of the file
    int nbLinesFile = 0;
    char line[255];
    while (fgets(line, sizeof(line), file) != NULL) {
        nbLinesFile++;
    }
    // On ferme le fichier
    fclose(file);
    // On alloue la mémoire pour le tableau de fichier avec le nombre de ligne
    char ** array = (char **) malloc(sizeof(char *) * nbLinesFile);
    // open the file
    file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    // On parcours le fichier
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // On alloue la mémoire pour la taille du nom du fichier
        array[i] = (char *) malloc(sizeof(char) * (strlen(line)+1));
        // On copie le nom du fichier dans le tableau
        strcpy(array[i], line);
        // On décrémente le compteur pour passer à la case du tableau suivante
        i++;
    }
    // fermeture du fichier
    fclose(file);
    // on renvoie le tableau tout allouer
    *nbLines = nbLinesFile;
    return array;
}

// fonction qui retourne le nombre de lignes d'un fichier et qui retourne le char* de chaque ligne
int isInArray(char * string, char ** array, int nbLines){
    // on parcours le tableau
    for (int i = 0; i < nbLines; i++) {
        // si la string est dans le tableau on renvoie true
        // commme dans le fichier il y a des \n à la fin de chaque ligne
        // on utilise strncmp pour comparer la valeur de la liste avec la taille du string
        if (strncmp(string, array[i], strlen(string))==0) {
            return 1;
        }
    }
    // si la string n'est pas dans le tableau on renvoie false
    return 0;
}
