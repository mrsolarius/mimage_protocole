#ifndef data_trame
#define data_trame

/**
 * Equivalent de erno
 */
extern int SPP_DATA_TRAME_Erno;

/**
 * Enum de toutes les erreurs
 */
enum SPP_DATA_TRAME_errCode{
    EMPTY_CMD=0,
    CMD_ERROR=1,
    EMPTY_STATUS=2,
    EMPTY_FD=3,
    FD_ERROR=4,
    EMPTY_SIZE_DATA=5,
    WRONG_SIZE=6
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
    UPLOAD_FILE_DATA=0xCD
};

/**
 * Enum de tous les status
 */
enum STATUS{
    SUCCESS =0x20, 
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
typedef struct infosTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned char nbFiles;
    unsigned char sizeInfos;
    char* infos;
};

/**
*   Nom: encodeInfosTrame - encode la structure encodeInfosTrame en binaire envoyé.
*   Description :
*      Récupère les informations contenus dans la structure InfosTrame et les encode dans le format de la trame pour une utilisation ultérieure.
*      Si infosTrame a un cmd inexistant, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*      Si infosTrame a un cmd ne corresponds pas à liste de commande,alors il envoie un message d'erreur 1 de type CMD_ERROR.
*      Si infosTrame a un status inexistant, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*      Si infosTrame a un sizeInfos est faux, alors il envoie un message d'erreur 6 de type WRONG_SIZE.
*
*      @param struct infosTrame 
*    EMPTY_CMD=0,
*    CMD_ERROR=1,
*    EMPTY_STATUS=2,
*    WRONG_SIZE=6,
*/

char* encodeInfosTrame(struct infosTrame);

/**
*   Nom: decodeInfosTrame - Décode le header de la trame et renvoie la structure de la trame.
*   Description :
*       Récupère les informations contenus dans la tête de la structure et les décode dans le format de la structure pour une utilisation ultérieure.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 1 de type CMD_ERROR.
*       Si la valeur de cmd est vide, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
*       Si la valeur de status est vide, alors il envoie un message d'erreur 2 de type EMPTY_STATUS.
*       Si infosTrame a un sizeInfos est faux, alors il envoie un message d'erreur 6 de type WRONG_SIZE.      
*      @param char* data corresponds au header de la trame.
*       CMD_ERROR=1,
*       EMPTY_CMD=0,
*       EMPTY_STATUS=2,
*       WRONG_SIZE=6
*/
struct infosTrame decodeInfosTrame(char* infos, unsigned int size);

/*-----------------------------------------FIN_INFOS_TRAME---------------------------------------------*/

/*-----------------------------------------DATA_TRAME---------------------------------------------*/
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
 *      Récupère les éléments de la tête de la structure et les réorganise selon la structure de la dataTrame préalablement définie.
 *      Si dataTrame a un cmd inexistant, alors il envoie un message d'erreur 0 de type EMPTY_CMD.
 *      Si dataTrame a un cmd ne corresponds pas à liste de commande,alors il envoie un message d'erreur 1 de type CMD_ERROR.
 *      Si status est vide, alors le programme renvoie une message d'erreur 2 de type EMPTY_STATUS.
 *      Si dataFd est vide, alors le programme renvoie une message d'erreur 3 de type EMPTY_FD.
 *      Si dataFd ne corresponds pas à liste de commande, alors il envoie un message d'erreur 4 de type FD_ERROR.
 *      Si sizeData est nul, alors  le programme renvoie une message d'erreur 5 de type EMPTY_SIZE_DATA.
 *  Paramètre :
 *      @param struct dataTrame corresponds à la structure générer par le serveur (à partir de l'analyse de fichier créer par celui -ci).

        EMPTY_CMD=0,
        CMD_ERROR=1,
        EMPTY_STATUS=2,
        EMPTY_FD=3
        FD_ERROR=4,
        EMPTY_SIZE_DATA=5,
 **/
char* encodeDataHead(struct dataTrame);

/**
*   Nom: decodeDataHead - Décode le header de la trame et renvoie une data trame contenant les informations récoltées.
*   Description :
*       Récupère les informations contenus dans la tête de la structure et les décode dans le format de la structure pour une utilisation ultérieure.
*       Si la valeur de cmd ne correspond pas à la liste de valeur possible, alors il envoie un message d'erreur 1 de type CMD_ERROR.
*       Si cmd est vide, alors le programme renvoie un message d'erreur 0 de type EMPTY_CMD.
*       Si status est vide, alors le programme renvoie une message d'erreur 2 de type EMPTY_STATUS.
*
*      @param char* data corresponds au header de la trame.
*       EMPTY_CMD=0,
*       CMD_ERROR=1,
*       EMPTY_STATUS=2,
*/
struct dataTrame decodeDataHead(char * data);

/*-----------------------------------------FIN_DATA_TRAME---------------------------------------------*/

#endif //data_trame.h