#ifndef SPP_IN_OUT_H
#define SPP_IN_OUT_H

#include "spp.h"

/**
  * Nom :
 *      sendDataTrame - envoie la dataTrame passé en paramètre. Il retourne 0 si succès sinon -1 erreur.
 * Description :
 *      Récupère les éléments de la structure et retourne rien. Il englobe en décode pour s'occuper du décodage des inforamtion dans la structure.
 *      Si dataTrame est vide alors il envoie un message d'erreur 0 de type dataTrameVide.
 *      Si dataTrame a un cmd inexistant, alors il il envoie un message d'erreur 1 de type cmdVide.
 *      Si dataTrame a un cmd ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 2 de type cmdErreur.
 *      Si dataTrame a un status inexistant, alors il il envoie un message d'erreur 3 de type statusVide.
 *      Si dataTrame a un status ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 4 de type statusErreur.
 *      Si dataTrame a un sizeData inexistant, alors il il envoie un message d'erreur 5 de type sizeDataVide.
 * Paramètre :
 *      @param PDataTrame corresponds à la structure générer par le serveur (à partir de l'analyse de fichier créer par celui -ci) 
 **/


int sendDataHead(PDataTrame);

int sendDataCore(PDataTrame);

PDataTrame receiveDataHead(int sockFd);
/**
  * Nom :
 *      receiveData - recoit le dataHead et un file descripteur. Il retourne 0 si succès sinon -1 erreur.
 * Description :
 *      recoit le dataHead et un descripteur et enregistre le fichier le fichier sur le disque. Il englobe en encode pour s'occuper de l'encodage des inforamtion dans la structure.
 *      
 *      Si dataTrame est vide alors il envoie un message d'erreur 0 de type dataTrameVide.
 *      Si dataTrame a un cmd inexistant, alors il il envoie un message d'erreur 0 de type cmdVide.
 *      Si dataTrame a un cmd ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 0 de type cmdErreur.
 *      Si dataTrame a un status inexistant, alors il il envoie un message d'erreur 0 de type statusVide.
 *      Si dataTrame a un status ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 0 de type statusErreur.
 *      Si dataTrame a un sizeData inexistant, alors il il envoie un message d'erreur 0 de type sizeDataVide.
 * Paramètre :
 *      @param char* head, int fileFD correspond à l'entete de dataTrame et le descripteur
 **/
int receiveDataCore(PDataTrame);


int sendInfo(PInfoTrame);

PInfoTrame receciveInfo(int sockFd);

#endif //SPP_IN_OUT_H