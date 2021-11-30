#ifndef type_h
#define type_h
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
char* getType(char * filePath) ;
#endif //type_h