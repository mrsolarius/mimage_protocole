#ifndef data_trame
#define data_trame

/**
 * Equivalent de erno
 */
extern int SPP_DATA_TRAME_Erno;

/**
 * Enum de toutes les erreur
 */
enum SPP_DATA_TRAME_errCode{
    dataTrameVide=0,
    cmdVide=1,
    cmdErreur=2,
    statusVide=3,
    statusErreur=4,
    sizeDataVide=5,
    sizeDataTaille=6
};


extern const char * SPP_DATA_TRAME_List[];
/**
 * Nom :
 *      SPP_perror - Afficher un message d'erreur de SPP
 * Description :
 *      La fonction SPP_perror() affiche un message sur la sortie d'erreur standard, décrivant la dernière erreur rencontrée lors de l'utilisation de SPP.
 *      D'abord, (si s n'est pas NULL et si *s n'est pas un octet nul), la chaîne de caractère s est imprimée, suivie de deux points (« : »),
 *      puis le message, suivi d'un saut de ligne.
 *
 *      La chaîne de caractères contient généralement le nom de la fonction où s'est produit l'erreur.
 *      Le numéro d'erreur est obtenu à partir de la variable externe SPP_Errno, qui contient le code d'erreur lorsqu'un problème survient,
 *      mais qui s'est pas effacée lorsqu'un appel est réussi.
 *
 *      La liste globale d'erreurs SPP_errList[] indexée par SPP_errno peut être utilisée pour obtenir le message d'erreur sans SPP_perror.
 *
 * Paramètre :
 *      @param msg correspond a message précédent l'erreur
 * 
 * Erreur :
 */
void SPP_perror(char * msg);


typedef struct dataTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned long sizeData;
    int dataFd;
};

/**
  * Nom :
 *      encodeDataHead - encode la structure dataTrame en binaire.
 * Description :
 *      Récupère les éléments de la structure et les réorganise selon la structure de la dataTrame préalablement définie.
 *      Si dataTrame est vide alors il envoie un message d'erreur 0 de type dataTrameVide.
 *      Si dataTrame a un cmd inexistant, alors il envoie un message d'erreur 1 de type cmdVide.
 *      Si dataTrame a un cmd ne corresponds pas à liste de commande,alors il envoie un message d'erreur 2 de type cmdErreur.
 *      Si dataTrame a un status inexistant, alors il envoie un message d'erreur 3 de type statusVide.
 *      Si dataTrame a un status ne corresponds pas à liste de commande,alors il envoie un message d'erreur 4 de type statusErreur.
 *      Si dataTrame a un sizeData inexistant, alors il envoie un message d'erreur 5 de type sizeDataVide.
 * Paramètre :
 *      @param struct dataTrame corresponds à la structure générer par le serveur (à partir de l'analyse de fichier créer par celui -ci).
 **/
char* encodeDataHead(struct dataTrame);

/**
*   Nom: decodeDataHead - Décode le header de la trame et renvoie une data trame contenant les informations récoltées.
*   Description :
*       Récupère les informations contenus dans la tête de la structure et les encode dans le format de la structure pour une utilisation ultérieure.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 2 de type cmdErreur.
*       Si la valeur status ne correspond pas à la liste de valeur possible, alors  il envoie un messase d'erreur 4 de type statusErreur.
*       Si la valeur sizeData ne correspond pas à la longeur de data dans la trame, alors il envoie une message d'erreur 6 de type sizeDataTaille.
*
*/
struct dataTrame decodeDataHead(char * data);


/**
  * Nom :
 *      sendDataTrame - envoie la dataTrame passé en paramètre.Il retourne 0 si succès sinon -1 erreur.
 * Description :
 *      Récupère les éléments de la structure et retourne rien.
 *      Si dataTrame est vide alors il envoie un message d'erreur 0 de type dataTrameVide.
 *      Si dataTrame a un cmd inexistant, alors il il envoie un message d'erreur 1 de type cmdVide.
 *      Si dataTrame a un cmd ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 2 de type cmdErreur.
 *      Si dataTrame a un status inexistant, alors il il envoie un message d'erreur 3 de type statusVide.
 *      Si dataTrame a un status ne corresponds pas à liste de commande,alors il il envoie un message d'erreur 4 de type statusErreur.
 *      Si dataTrame a un sizeData inexistant, alors il il envoie un message d'erreur 5 de type sizeDataVide.
 * Paramètre :
 *      @param struct dataTrame corresponds à la structure générer par le serveur (à partir de l'analyse de fichier créer par celui -ci) 
 **/


int sendDataTrame( struct dataTrame);

/**
  * Nom :
 *      receiveData - recoit le dataHead et un file descripteur. Il retourne 0 si succès sinon -1 erreur.
 * Description :
 *      recoit le dataHead et le descripteur 
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
int receiveData(char*  head, int fileFD);




#endif //data_trame.h