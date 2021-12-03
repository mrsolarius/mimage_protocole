#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interface.h"

void liste(char ** nom, int taille){
    fprintf(stdout,"Voici la liste des fichiers présents sur le serveur :\n");
    for(int comp=0;comp<taille;comp++){
        fprintf(stdout,"\t>%d - %s\n",comp,nom[comp]); 
    }
}

void envoie(){
}

char ** recuperation(char ** nom, int taille){
    char * saisie= malloc(sizeof(char));
    fprintf(stdout,"\tQuel fichier voulez vous récupérer ? Merci d'entrer le numéro correspondant:\n");
    fscanf(stdin,"%c",saisie);
    saisie=atoi(saisie);
    while((saisie>taille)||(saisie<0)){
        fprintf(stdout,"Mauvais numéro de fichier. Merci d'entrer un numéro compris dans la liste:");
        fscanf(stdin,"%c",saisie);
        saisie=atoi(saisie);
    };
    char ** retour= (char **) malloc(sizeof(char));
    
    return 0;
}

void menu(){
    printf("\n"CHD MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH CHG"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(MV "\tBienvenue sur le client de gestion de fichiers d'images. Veuillez saisir le numéro de l'action voulue :\t\t "MV"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(MV "\t\t\t1 - Consulter la liste de fichier\t\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t2 - Envoyer un fichier sur le serveur\t\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t3 - Récupérer un fichier depuis le serveur\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t4 - Arrêter le client et quitter la connexion\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(CBD MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH CBG"\n");
}


void Affiche(){
    char * choix= malloc(sizeof(char));

    //exemples de noms
    char str[] ="voiture";
    char ** files = (char **) malloc(sizeof(char *) * 2);
    files[0] = (char *) malloc(sizeof(char) * (strlen(str)+1));
    strcpy(files[0],str);  
    
    char str2[] ="camion";
    files[1] = (char *) malloc(sizeof(char) * (strlen(str)+1));
    strcpy(files[1],str2); 
    int taille=2;
    
    
    int quit=0;
    do{
        menu();
        fscanf(stdin,"%s",choix);
        switch  (atoi(choix)){
            case 1:
                liste(files,taille);
                break;
            case 2:
                envoie();
                break;
            case 3:
                liste(files,taille);
                recuperation(files,taille);
                break;
            case 4:
                quit=1;
                break;
            default:
                fprintf(stdout,"\n\t\t\t\t/!\\ Merci d'entrer une valeur présente dans la liste ! /!\\ \n");
        };
    }
    while(quit!=1);
}
