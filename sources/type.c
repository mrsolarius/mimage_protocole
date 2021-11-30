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

/*
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int pipePsGrep[2],pipeGrepWc[2];
    pipe(pipePsGrep);
    switch(fork()){
        case -1:
            perror("Fork error");
            exit(-1);
        //premier process fis
        case 0:
            //Fermer l'entrer du pipe
            close(pipePsGrep[0]);

            //Femer stdout
            close(1);
            //on clone mon pipe dans le stdout
            dup(pipePsGrep[1]);
            close(pipePsGrep[1]);
            if(execlp("ps","ps","ax",NULL)!=-1){
                perror("Fork error");
                exit(-1);
            }
    }
    wait(NULL);
    pipe(pipeGrepWc);
    switch(fork()){
        case -1:
            perror("Fork error");
            exit(-1);
        case 0:
            //Fermer l'entrer du pipe car on utilise la sortie
            close(pipePsGrep[1]);

            //Femer stdin
            close(0);
            dup(pipePsGrep[0]);
            close(pipePsGrep[0]);

            //fermeture de la sortie du pipe ver wc car on ne l'utilise pas
            close(pipeGrepWc[0]);
            //Femer stdout
            close(1);
            dup(pipeGrepWc[1]);
            close(pipeGrepWc[1]);
            if(execlp("grep","grep","bash",NULL)!=-1){
                perror("Fork error");
                exit(-1);
            }
    }
    close(pipePsGrep[1]);
    close(pipePsGrep[0]);
    wait(NULL);
    switch(fork()){
        case -1:
            perror("Fork error");
            exit(-1);
        //premier process fis
        case 0:
            //Fermer la sortie du pipe
            close(pipeGrepWc[1]);

            //Femer stdout
            close(0);
            //on clone mon pipe l'entrer du pipe dans le stdout
            dup(pipeGrepWc[0]);
            close(pipeGrepWc[0]);
            if(execlp("wc","wc","-l",NULL)!=-1){
                perror("Fork error");
                exit(-1);
            }
    }
    close(pipeGrepWc[1]);
    close(pipeGrepWc[0]);
    wait(NULL);
    exit(0);
}
*/