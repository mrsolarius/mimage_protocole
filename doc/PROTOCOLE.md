# Unamed Protocole
## Fonctionnalités attendues
* consulter la liste des fichier disponibles
* déposer un fichier sur le server
* récupérer un fichier du server
* fermer la connexion avec le server

## Consulter la liste des fichiers disponibles
### Résumé
Pour consulté la listes des fichiers disponibles. Le client commance par envoyer un message de type `ASK_LIST` au serveur.
Le server répond avec un message de type `LIST_SIZE` contenant le nombre de trame de fichier à récupérer.
Il poursuit ensuite par l'envoie d'autant de messages de type `SHOW_FILE` qu'indiquer dans le message `LIST_SIZE`.
```
Client                  Serveur
   |         GET_LIST      |
   |---------------------->|
   |      LIST_SIZE        |
   |<----------------------|
   |   DOWNLOAD_FILE_NAME  |
   |<----------------------|
   |          .            |
   |          .            |
   |          .            |
   |   DOWNLOAD_FILE_NAME  |
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


* `DOWNLOAD_FILE_NAME`: Message de type `DOWNLOAD_FILE_NAME` envoyé au client.
  * `CMD` : Commande indiquant le but de la requête.
    * Ici, 0xA2 
  * `SIZE_NAME` : Longeur de la chaine de caracther.
    * Ici, Unsigned Char 
  * `NAME` : Chaine de caracther.
    * Ici, Char*  

## Déposer un fichier sur le server
### Résumé
Pour dépose un fichier sur le serveur. Le client envoie un message de type `PUT_FILE` au serveur. 
Le serveur répond alors par un message de type `ASERT_PUT_FILE` pour confirmer ou annuler le dépôt.
Si le server accepte le dépôt, le client envoie alors le fichier dans un message de type `PUT_FILE_DATA`.
Le serveur répond alors par un message de type `ASERT_PUT_FILE_DATA` pour confirmer ou annuler le dépôt.
```
Client                  Serveur
   |     UPLOAD_FILE_NAME  |
   |---------------------->|
   |     ACQUIT_FILE_NAME  |
   |<----------------------|
   |    UPLOAD_FILE_DATA   |
   |---------------------->|
   |    ACQUIT_FILE_DATA   |
   |<----------------------|
```
### Contenu des messages
* `UPLOAD_FILE_NAME`: Message de type `UPLOAD_FILE_NAME` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC2
    * `SIZE_NAME` : Taille du nom du fichier.
      * Ici, Unsigned Char
    * `NAME` : Nom du fichier.
      * Ici, Char*


* `ACQUIT_FILE_NAME`: Message de type `ACQUIT_FILE_NAME` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA3
   * `STATUS` : Statut de la requête.
     * 0x20 : Accepté
     * 0x41 : Refusé : Nom de fichier invalide
     * 0x43 : Refusé : Nom de fichier déjà pris
     * 0x50 : Refusé : Erreur internes du serveur
     * 0x52 : Refusé : Fichier trop volumineux
   * `SIZE_NAME` : Taille du nom du fichier.
     * Ici, Unsigned Char
   * `NAME` : Nom du fichier.
     * Ici, Char*


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
Pour récupérer un fichier du server. Le client envoie un message de type `GET_FILE` au serveur. 
Le serveur répond alors par un message de type `PUT_FILE` envoyer le fichier.
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
    * `SIZE_NAME` : Taille du nom du fichier.
      * Ici, Unsigned Char
    * `NAME` : Nom du fichier.
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
* 0xA2 : DOWNLOAD_FILE_NAME
* 0xA3 : ACQUIT_FILE_NAME
* 0xA4 : ACQUIT_FILE_DATA
* 0xAD : DOWNLOAD_FILE_DATA

#### Commandes client
* 0xC1 : GET_LIST
* 0xC2 : UPLOAD_FILE_NAME
* 0xC3 : GET_FILE_DATA
* 0xCD : UPLOAD_FILE_DATA
* 0xCE : CLOSE_SOCKET

## Les Trames
Nous avons ici décider de mettre en place deux trame différentes.
Une trame nommee `INFOS_TRAME` qui contient les information conexe à l'envoie de fichier.
Et une trame nommee `DATA_TRAME` qui contient l'entête du fichier.

### Trame `INFOS_TRAME`
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
