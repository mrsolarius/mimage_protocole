#ifndef utils_h
#define utils_h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
/**
 * getType utilise un exec sur la commande File 
 * et lit dans l'entrée standard pour récupérer 
 * le type de fichier
*/
char* getType(char * filePath);
int countFiles(char * path);
char ** getFiles(char * path, int nbFiles);
char ** fileToArray(char * filePath, int * nbLines);
int isInArray(char * string, char ** array, int nbLines);
#endif //utils_h