# Send Pic Protocole (SPP)
## Fonctionnalités attendues
* consulter la liste des fichier disponibles
* déposer un fichier sur le server
* récupérer un fichier du server
* fermer la connexion avec le server

## Consulter la liste des fichiers disponibles
### Résumé
Pour consulté la listes des fichiers disponibles. Le client commance par envoyer un message de type `GET_LIST` au serveur.
Le server répond avec un message de type `LIST_SIZE` contenant le nombre de trame de fichier à récupérer.
Il poursuit ensuite par l'envoie d'autant de messages de type `DOWNLOAD_FILE_INFO` qu'indiquer dans le message `LIST_SIZE`.
```
Client                  Serveur
   |         GET_LIST      |
   |---------------------->|
   |      LIST_SIZE        |
   |<----------------------|
   |   DOWNLOAD_FILE_INFO  |
   |<----------------------|
   |          .            |
   |          .            |
   |          .            |
   |   DOWNLOAD_FILE_INFO  |
   |<----------------------|
```
### Contenu des messages
* `GET_LIST`: Message de type `GET_LIST` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC1 


* `LIST_SIZE`: Message de type `LIST_SIZE` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA1
   * `NB_FILE` : Nombre de fichier à récupérer.
     * Ici, Integer 


* `DOWNLOAD_FILE_INFO`: Message de type `DOWNLOAD_FILE_INFO` envoyé au client.
  * `CMD` : Commande indiquant le but de la requête.
    * Ici, 0xA2 
  * `SIZE_INFO` : Longeur de la chaine de caractères.
    * Ici, Unsigned Char 
  * `INFO` : Chaine de caractères.
    * Ici, Char*  

## Déposer un fichier sur le server
### Résumé
Pour déposer un fichier sur le serveur. Le client envoie un message de type `UPLOAD_FILE_INFO` au serveur. 
Le serveur répond alors par un message de type `ACQUIT_FILE_INFO` pour confirmer ou annuler le dépôt.
Si le serveur accepte le dépôt, le client envoie alors le fichier dans un message de type `UPLOAD_FILE_DATA`.
Le serveur répond alors par un message de type `ACQUIT_FILE_DATA` pour confirmer ou annuler le dépôt.
```
Client                  Serveur
   |    UPLOAD_FILE_INFO   |
   |---------------------->|
   |    ACQUIT_FILE_INFO   |
   |<----------------------|
   |    UPLOAD_FILE_DATA   |
   |---------------------->|
   |    ACQUIT_FILE_DATA   |
   |<----------------------|
```
### Contenu des messages
* `UPLOAD_FILE_INFOS`: Message de type `UPLOAD_FILE_INFOS` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC2
    * `SIZE_INFO` : Taille du nom du fichier.
      * Ici, Unsigned Char
    * `INFO` : Nom du fichier.
      * Ici, Char*


* `ACQUIT_FILE_INFO`: Message de type `ACQUIT_FILE_INFO` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA3
   * `STATUS` : Statut de la requête.
     * 0x20 : Accepté
     * 0x41 : Refusé : Nom de fichier invalide
     * 0x43 : Refusé : Nom de fichier déjà pris
     * 0x50 : Refusé : Erreur internes du serveur
     * 0x52 : Refusé : Fichier trop volumineux


* `UPLOAD_FILE_DATA`: Message de type `UPLOAD_FILE_DATA` envoyé au serveur.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xCD
   * `SIZE_DATA` : Taille du fichier.
     * Ici, Long
   * `DATA` : Fichier.
     * Ici, Char*


* `ACQUIT_FILE_DATA`: Message de type `ACQUIT_FILE_DATA` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA4
   * `STATUS` : Statut de la requête.
     * 0x20 : Accepté
     * 0x42 : Refusé : Extension de fichier invalide
     * 0x45 : Refusé : Fichier ne respect pas la checksum
     * 0x50 : Refusé : Erreur interne
     * 0x51 : Refusé : Plus de place disponible


## Récupérer un fichier du server
### Résumé
Pour récupérer un fichier du server. Le client envoie un message de type `GET_FILE_DATA` au serveur. 
Le serveur répond alors par un message de type `DOWNLOAD_FILE_DATA` envoyer le fichier.
```
Client                  Serveur
   |     GET_FILE_DATA     |
   |---------------------->|
   |   DOWNLOAD_FILE_DATA  |
   |<----------------------|
```
### Contenu des messages
* `GET_FILE_DATA`: Message de type `GET_FILE_DATA` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC3
    * `SIZE_INFO` : Taille du nom du fichier.
      * Ici, Unsigned Char
    * `INFO` : Nom du fichier.
      * Ici, Char*

    
* `DOWNLOAD_FILE_DATA`: Message de type `DOWNLOAD_FILE_DATA` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xAD
   * `STATUS`:
     * 0x20 : Accepté
     * 0x44 : Refusé : Fichier introuvable
     * 0x50 : Refusé : Erreur interne
   * `SIZE_DATA` : Taille du fichier.
     * Ici, Long
   * `DATA` : Fichier.
     * Ici, Char*

## Fermer la connexion avec le serveur
### Résumé
Pour informer le serveur d'une deconexion. Le client envoie un message de type `CLOSE_SOCKET` au serveur. 
Le serveur fermer alors le processus de communication.
```
Client                  Serveur
   |      CLOSE_SOCKET     |
   |---------------------->|
   |                       X
   |
```
### Contenu des messages
* `CLOSE_SOCKET`: Message de type `CLOSE_SOCKET` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xCE

## Champ de type `STATUS`
* 0x20 : Accepté
* 0x41 : Refusé : Nom de fichier invalide
* 0x42 : Refusé : Extension de fichier invalide
* 0x43 : Refusé : Nom de fichier déjà pris
* 0x44 : Refusé : Fichier introuvable
* 0x45 : Refusé : Fichier ne respect pas la checksum
* 0x50 : Refusé : Erreur interne
* 0x51 : Refusé : Plus de place disponible
* 0x52 : Refusé : Fichier trop volumineux

## Champ de type `CMD`
### Le A signifie que c'est une commande serveur, le C que c'est une commande client, le D que nous envoyons la donnée du fichier
### Les noms ont été choisi en nous placant du point de vu de l'utilisateur (upload du client au serveur / download du serveur au client)
#### Commandes serveur
* 0xA1 : LIST_SIZE
* 0xA2 : DOWNLOAD_FILE_INFO
* 0xA3 : ACQUIT_FILE_INFO
* 0xA4 : ACQUIT_FILE_DATA
* 0xAD : DOWNLOAD_FILE_DATA

#### Commandes client
* 0xC1 : GET_LIST
* 0xC2 : UPLOAD_FILE_INFO
* 0xC3 : GET_FILE_DATA
* 0xCD : UPLOAD_FILE_DATA
* 0xCE : CLOSE_SOCKET

## Les Trames
Nous avons ici décider de mettre en place deux trame différentes.
Une trame nommee `INFO_TRAME` qui contient les informations connexes à l'envoie de fichier.
Et une trame nommee `DATA_TRAME` qui contient l'entête du fichier.

### Trame `INFO_TRAME`
```
0                 1                 2
 0 1 2 3 4 5 6 7 8 0 1 2 3 4 5 6 7 8 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       CMD       |      STATUS     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      NB_FILE    |    SIZE_INFO    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                INFO            -->|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

### Trame `DATA_TRAME`
```
0                 1                 2
 0 1 2 3 4 5 6 7 8 0 1 2 3 4 5 6 7 8
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      CMD        |      STATUS     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             SIZE_DATA             |
|              4 Octet              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               DATA             -->|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

## Implémentation en C
Pour implémenter le protocole en C nous avons effectué différents choix.

En effet, nous avons décidé de mettre en place un système de sérialisation et désérialisation des trames.
Cela nous permet de plus simplement envoyer des trames  sous forme de `unsigned char*` et de les récupérer 
et les traiter sous forme de structure plus simple à manipuler.

### Structure
Afin de coller au mieux au protocole nous avons décidé de faire deux structures différentes, une par trame.
Pour chaque structure, toujours par simplicité, nous avons décidé de mettre en place un type custom
permettant d'accéder au pointeur de la trame.

Voici le code des structures :

Pour les trames de type `INFO_TRAME`
```c
typedef struct _InfosTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned char nbFiles;
    unsigned char sizeInfos;
    char* infos;
} InfosTrame;

typedef InfosTrame * PInfoTrame;
```
Pour les trames de type `DATA_TRAME`
```c
typedef struct _DataTrame{
    unsigned char cmd;
    unsigned char status;
    unsigned int sizeData;
    int dataFd;
} DataTrame;

typedef DataTrame * PDataTrame;
```

On y retrouve les champs présents dans les trames. Sauf pour la trame DATA_TRAME, 
qui contient en plus le descripteur de fichier des données transmises car elle ne les contient pas directement.

### Enumération
Nous avons décidé de mettre en place une enumération pour les commandes et les statuts car ces derniers
sont des constantes définies dans le protocole.

Voici donc les enumérations :
```c
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
```

```c
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
```
### Sérialisation
Les fonctions de sérialisation nous permettent de convertir une structure en `unsigned char*` dans le but de l'envoyer.
Elles ont aussi pour but d'encapsuler les données qu'elles contiennent afin de rester cohérent avec le protocole.

Ici nous avons décidé de mettre en place deux fonctions de sérialisation (encodeInfosTrame() et encodeDataHead()).
Une pour les trames de type `INFO_TRAME` et une pour les trames de type `DATA_TRAME`.

Chacune de ces fonctions fait appel à une fonction de vérification de l'encapsulation des données (checkDataTrameError() et checkInfoTrameError()).
Ces fonctions d'encapsulation d'erreur ont pour but de générer des erreurs d'execution si les données ne sont pas cohérentes,
afin de nous informer de potentielles mauvaises utilisations.

La sérialisation des trames de type `DATA_TRAME` est un peu particuliére, en effet on ne sérialise pas les données directement,
seulement l'entête de la trame. Les données, quant à elles, sont envoyées et traitées par le serveur ou le client directement 
dans le socket, après l'envoi de l'entête grâce au descripteur de fichier présent dans la structure `DataTrame`.

### Désérialisation
Les fonctions de désérialisation nous permettent de convertir une trame `unsigned char*` en structure dans le but de la traiter plus simplement dans le code.
Elle sont l'exact inverse des fonctions de sérialisation. La particularité ici est que nous testons l'encapsulation des données
une fois qu'elles sont désérialisées avec les mêmes fonctions que pour la sérialisation.

Ici nous avons déciidé de mettre en place deux fonctions de désérialisation (decodeInfosTrame() et decodeDataHeadTrame()).
La particularité ici est que decodeDataHeadTrame() demande en paramètre le descripteur de fichier du socket pour le placer
dans la structure.

### Gestion des erreurs
Pour la gestion des erreurs, nous nous sommes inspirés de la façon dont les erreurs sont gérées avec Erno.
Cela veut dire que nous avons mis en place une variable globale `SPP_Erno` qui contient les exceptions rencontrées.
SPP_Erno est par défaut à -1. Lorsque une erreur est rencontrée, la fonction va la placer sur le bon code d'erreur. 
Code d'erreur corespondant à l'emplacement du message d'erreur dans le tableau `DDP_errList[]`. 
Ce qui nous laisse ensuite deux options :
* faire appeler la fonction `SPP_perror()` pour avoir le message d'erreur correspondant au code d'erreur
* directement l'imprimer dans la sortie erreur avec `DDP_errList[SPP_Erno]`.

Les erreur SPP sont principalement renvoyées par les fonctions de vérification de l'encapsulation des données.
Les fonction de sérialisation et de désérialisation peuvent ensuite les propager en renvoyant un code d'erreur égal à
0xff sur l'emplacement de la commande (soit frame[0], soit struc->cmd).
Si le code d'erreur est détécté, on peut alors faire appel à la fonction `SPP_perror()` pour avoir le message 
d'erreur correspondant.

# Test Driven Development
Afin de faciliter la vérification de la fonctionnalité, nous avons décidé de faire du TDD.
Cela veut dire que nous avons commencé par rédiger nos headers, puis les tests unitaires des fonctions que nous avons prototypées, et enfin nous 
les avons implémentées en c en veillant à respecter les spécifications des tests.


Pour se faire, nous avons mis en place un dossier `test` qui contient toutes fonction de test. 
Comme nous ne connaissons pas les frameworks de tests unitaires, nous avons mis en place notre propre 
core de test dans le fichier `test-core.c`.

Ce core de test contient les fonctions de test de la fonctionnalité. Ainsi que quelques fonctions d'affichage utile.
Le principe ici est le suivant : 
 * Un fichier de test par fichier c à tester
 * Chaque fonction à tester doit avoir une ou plusieurs fonctions de test qui lui est associé.
 * Chaque fonction de test renvoie un boolean qui doit être vrai si le test est passé.
 * Chaque fonction de test doit avoir un commentaire qui explique ce qu'elle fait.
 * Toutes les fonctions de test sont appelées par une fonction de fichier C.
 * Cette fonction de fichier C est déclarée dans `all_tests.h` et appelée dans `all_tests.c`.

Pour lancer les tests, nous nous sommes arrangés pour que notre makefile soit en mesure de les build spéarément du code utilisable.

Nous avons donc crée la phony rule `test` qui permet de build les fichiers de test. Ensuite on peut lancer les tests
en appelant le fichier `run_tests` dans le dossier bin.