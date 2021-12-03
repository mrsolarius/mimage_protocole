#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interface.h"

void liste(char ** nom, int taille){
    fprintf(stdout,"\tVoici la liste des fichiers présents sur le serveur :\n");
    for(int comp=0;comp<taille;comp++){
        fprintf(stdout,"\t\t>%d - %s\n",comp,nom[comp]); 
    }
}

char * envoie(){
    char *chemin;
    printf("\tMerci de renseigner le chemin du fichier que vous souhaitez envoyer :");
    return chemin;
}

void ligne(){
    printf("============================================================================================================================\n");
}

void curseur(){
    printf("\n\t>:");
}

int recuperation(char ** nom, int taille){
    int saisie;
    char lect[255];
    fprintf(stdout,"\tQuel fichier voulez vous récupérer ? Merci d'entrer le numéro correspondant:");
    curseur();
    fscanf(stdin,"%s",lect);
    saisie=atoi(lect);
    while((saisie>taille)||(saisie<0)){
        fprintf(stdout,"\t\t\t\t/!\\ Mauvais numéro de fichier. Merci d'entrer un numéro compris dans la liste:\t /!\\");
        curseur();
        liste(nom,taille);
        fscanf(stdin,"%s",lect);
        saisie=atoi(lect);
    };
    return saisie;
}

void menu(){
    printf("\n"CHD MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH CHG"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(MV "\tBienvenue sur le client de gestion de fichiers d'images. Veuillez saisir le numéro de l'action voulue :\t\t "MV"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(MV "\t\t\t1 - Consulter la liste de fichier\t\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t2 - Envoyer un fichier sur le serveur\t\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t3 - Récupérer un fichier depuis le serveur\t\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(MV "\t\t\t0 [Default] - Arrêter le client et quitter la connexion\t\t\t\t\t\t " MV"\n");
    printf(MV "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t "MV"\n");
    printf(CBD MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH MH CBG);
}


void Affiche(){
    char * choix= malloc(sizeof(char));

    //exemples de noms
    char str[] ="voiture.png";
    char ** files = (char **) malloc(sizeof(char *) * 2);
    files[0] = (char *) malloc(sizeof(char) * (strlen(str)+1));
    strcpy(files[0],str);  
    
    char str2[] ="camion.ico";
    files[1] = (char *) malloc(sizeof(char) * (strlen(str)+1));
    strcpy(files[1],str2); 
    int taille=2;
    
    int index;
    int quit2=1;
    int quit=0;
    do{
        menu();
        curseur();
        fscanf(stdin,"%s",choix);
        switch  (atoi(choix)){
            case 1:
                ligne();
                liste(files,taille);
                ligne();
                break;
            case 2:
                ligne();
                envoie();
                ligne();
                break;
            case 3:
                do{
                    ligne(); 
                    liste(files,taille);
                    index=recuperation(files,taille); //on récupère l'indice du nom du fichier à récup
                    //mettre le téléchargement du fichier ici @todo
                    printf("\t%s\n\tVoulez-vous récupérer un autre fichier ?\n\t\tEntrer 0 pour revenir au menu principal.\n\t\tEntrer 1 pour récupérer un autre fichier.",files[index]);
                    curseur();
                    scanf("%s",choix);
                    if ((atoi(choix)!=1&&atoi(choix)!=0)){
                        printf("\t\t/!\\Merci de rentrer une valeur valide /!\\");
                        curseur();
                        scanf("%s",choix);
                    }
                    quit2=atoi(choix);
                    ligne();
                }
                while(quit2!=0);
                break;
            case 0:
                quit=1;
                break;
            default:
                fprintf(stdout,"\n\t\t\t\t/!\\ Merci d'entrer une valeur présente dans la liste ! /!\\");
                break;
        };
    }
    while(quit!=1);
}
