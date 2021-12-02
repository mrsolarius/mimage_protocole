#ifndef data_trame
#define data_trame
#include <stdbool.h>

/**
 * Equivalent de erno
 */
extern int SPP_Erno;

/**
 * Enum de toutes les erreurs
 */
enum SPP_ERRCODE{
    EMPTY_CMD=0,
    CMD_ERROR=1,
    EMPTY_STATUS=2,
    EMPTY_FD=3,
    FD_ERROR=4,
    EMPTY_SIZE_DATA=5,
    WRONG_SIZE=6,
    STATUS_ERROR=7
};

/**
 * Enum de toutes les commandes
 */
enum COMMANDES {
    LIST_SIZE=0xA1, 
    DOWNLOAD_FILE_NAME=0xA2, 
    ACQUIT_FILE_NAME=0xA3,
    ACQUIT_FILE_DATA=0xA4,
    DOWNLOAD_FILE_DATA=0xAD,
    GET_LIST=0xC1,
    UPLOAD_FILE_NAME=0xC2,
    GET_FILE_DATA=0xC3,
    UPLOAD_FILE_DATA=0xCD,
    CLOSE_SOCKET=0xCE
};

/**
 * Enum de tous les status
 */
enum STATUS{
    SUCCESS=0x20, 
    INVALID_NAME_FILE= 0x41,
    INVALID_EXTEND_FILE= 0x42, 
    NAME_ALREADY_TAKEN = 0x43,
    NO_FOUND_FILE= 0x44,
    CHECKSUM_ERROR= 0x45,
    INTERNAL_ERROR= 0x50,
    LACK_OF_SPACE= 0x51,
    FILE_TOO_LARGE= 0x52
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
 * 
 */
void SPP_perror(char * msg);
/*-----------------------------------------INFOS_TRAME---------------------------------------------*/
typedef struct _InfosTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned char nbFiles;
    unsigned char sizeInfos;
    char* infos;
} InfosTrame;

typedef InfosTrame * PInfoTrame;
/**
*   Nom: encodeInfosTrame - encode la structure encodeInfosTrame en binaire envoyé.
*   Description :
*      Récupère les informations contenus dans la structure InfosTrame et les encode dans le format de la trame pour une utilisation ultérieure.
*      Si infosTrame a un cmd inexistant, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*      Si infosTrame a un cmd ne corresponds pas à liste de commande,alors il envoie un message d'erreur 1 de type CMD_ERROR.
*      Si infosTrame a un status inexistant, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*      Si la valeur de status ne correspond pas aux valeurs de la liste, alors il envoie un message d'erreur 7 de type STATUS_ERROR.
*      Si infosTrame a un sizeInfos est faux, alors il envoie un message d'erreur 6 de type WRONG_SIZE.
*
*      @param struct infosTrame 
*    EMPTY_CMD=0,
*    CMD_ERROR=1,
*    EMPTY_STATUS=2,
*    STATUS_ERROR=7,
*    WRONG_SIZE=6,
*/

unsigned char* encodeInfosTrame(PInfoTrame);

/**
*   Nom: decodeInfosTrame - Décode le headers de la trame et renvoie la structure de la trame.
*   Description :
*       Récupère les informations contenus dans la tête de la structure et les décode dans le format de la structure pour une utilisation ultérieure.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 1 de type CMD_ERROR.
*       Si la valeur de cmd est vide, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*       Si la valeur de status est vide, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*       Si la valeur de status ne correspond pas aux valeurs de la liste, alors il envoie un message d'erreur 7 de type STATUS_ERROR.
*       Si infosTrame a un sizeInfos est faux, alors il envoie un message d'erreur 6 de type WRONG_SIZE.      
*      @param char* data corresponds au headers de la trame.
*       CMD_ERROR=1,
*       EMPTY_CMD=0,
*       EMPTY_STATUS=2,
*       STATUS_ERROR=7,
*       WRONG_SIZE=6,
*       STATUS_ERROR=7
*/
PInfoTrame decodeInfosTrame(unsigned char* infos, unsigned int size);

/**
*   Nom: checkInfoTrameError - Test les erreurs potentiels de la trame infotrame et positionne SPP_ERNO.
*   Description :
*       Test la cohérences des valeurs et renvoie une booléen indiquent si une valeur ne l'est pas.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 1 de type CMD_ERROR.
*       Si la valeur de cmd est vide, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*       Si la valeur de status est vide, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*       Si la valeur de status ne correspond pas aux valeurs de la liste, alors il envoie un message d'erreur 7 de type STATUS_ERROR.
*       Si infosTrame a un sizeInfos qui est différent de sa taille, alors il envoie un message d'erreur 6 de type WRONG_SIZE.      
*      @param char* data corresponds au headers de la trame.
*       CMD_ERROR=1,
*       EMPTY_CMD=0,
*       EMPTY_STATUS=2,
*       STATUS_ERROR=7,
*       WRONG_SIZE=6
*/
bool checkInfoTrameError(PInfoTrame);

/*-----------------------------------------FIN_INFOS_TRAME---------------------------------------------*/

/*-----------------------------------------DATA_TRAME---------------------------------------------*/
typedef struct _DataTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned int sizeData;
    int dataFd;
} DataTrame;
typedef DataTrame * PDataTrame;
/**
  * Nom :
 *      encodeDataHead - encode la structure dataTrame en binaire.
 * Description :
 *      Récupère les éléments de la tête de la structure et les réorganise selon la structure de la dataTrame préalablement définie.
 *      On vérifie si la trame est bien implémenté.
 *      On vérifie si la trame est correct avec un status erreur.
 *      On vérifie si il renvoie bien une erreur.
 *  Paramètre :
 *      @param struct dataTrame corresponds à la structure générer par le serveur (à partir de l'analyse de fichier créer par celui -ci).
 *      CorrectFrame
 *      CorrectFrameWithStatusError
 *      ThrowError
 **/
unsigned char* encodeDataHead(PDataTrame);

/**
*   Nom: decodeDataHead - Décode le headers de la trame et renvoie une data trame contenant les informations récoltées.
*   Description :
*       Récupère les informations contenus dans la tête de la structure et les décode dans le format de la structure pour une utilisation ultérieure.
*       On vérifie le cas ou il y a une erreur dans le cmd.
*       On vérifie le cas ou le status erreur est vide.
*       On vérifie si la trame fonctionne avec les datas.
*       On vérifie si la trame fonctionne avec des erreurs.
*
*      @param char* data corresponds au headers de la trame.
*       CmdERORR
*       EMPTY_STATUSError
*       PassWithData
*       PassWithError
*       
*/
PDataTrame decodeDataHead(unsigned char * data, int dataFd);

/**
*   Nom: checkDataTrameError - Test les erreurs potentiels de la trame datatrame et positionne SPP_ERNO.
*   Description :
*       Test la cohérences des valeurs et renvoie une booléen indiquent si une valeur ne l'est pas.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 1 de type CMD_ERROR.
*       Si la valeur de cmd est vide, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*       Si la valeur de status est vide, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*       Si la valeur de status ne correspond pas aux valeurs de la liste, alors il envoie un message d'erreur 7 de type STATUS_ERROR.
*       Si dataTrame a un dataFd nul, alors il envoie un message d'erreur 3 de type EMPTY_FD.
*       Si dataTrame a une dataFd inférieur à 0, alors il envoie un message d'erreur 4 de type FD_ERROR.
*       Si dataTrame a un sizeData de 0 et que le status est SUCESS, alors il envoie un message d'erreur 5 de type EMPTY_SIZE_DATA.
*       On vérifie si la trame fonctionne avec des datas.
*       On vérifie si la trame revoie des erreurs.
*      @param char* data corresponds au headers de la trame.
*       
*       EMPTY_CMD=0,
*       CMD_ERROR=1,
*       EMPTY_STATUS=2,
*       STATUS_ERROR=7,
*       EMPTY_FD=3,
*       EMPTY_SIZE_DATA=5
*       FD_ERROR=4,
*       WithDATA()
*       WithError
*/
bool checkDataTrameError(PDataTrame dataTrame);

/*-----------------------------------------FIN_DATA_TRAME---------------------------------------------*/

/*-------------------------------------------DEBUT_UTILE----------------------------------------------*/

//Fonction qui permet de connaitre le type de la trame reçu
int checkTypeFrame(unsigned char* data);

void SPP_perror(char* msg);
#endif //data_trame.h