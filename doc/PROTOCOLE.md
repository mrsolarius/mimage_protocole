# Unamed Protocole
## Fonctionnalités attendues
* consulter la liste des fichier disponibles
* déposer un fichier sur le server
* récupérer un fichier du server

## Consulter la liste des fichiers disponibles
### Résumé
Pour consulté la listes des fichiers disponibles. Le client commance par envoyer un message de type `LIST` au serveur.
Le server répond avec un message de type `LIST_PREFIX` contenant le nombre de trame de fichier à récupérer.
Il poursuit ensuite par l'envoie d'autant de messages de type `SHOW_FILE` qu'indiquer dans le message `LIST_PREFIX`.
```
Client                  Serveur
   |         LIST          |
   |---------------------->|
   |      LIST_PREFIX      |
   |<----------------------|
   |       SHOW_FILE       |
   |<----------------------|
   |          .            |
   |          .            |
   |          .            |
   |       SHOW_FILE       |
   |<----------------------|
```
### Contenu des messages
* `LIST`: Message de type `LIST` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC1 


* `LIST_PREFIX`: Message de type `LIST_PREFIX` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA1
   * `NB_FILE` : Nombre de fichier à récupérer.
     * Ici, Integer 


* `SHOW_FILE`: Message de type `SHOW_FILE` envoyé au client.
  * `CMD` : Commande indiquant le but de la requête.
    * Ici, 0xA2 
  * `SIZE_NAME` : Longeur de la chaine de caracther.
    * Ici, Integer 
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
   |        PUT_FILE       |
   |---------------------->|
   |     ASERT_PUT_FILE    |
   |<----------------------|
   |     PUT_FILE_DATA     |
   |---------------------->|
   |  ASERT_PUT_FILE_DATA  |
   |<----------------------|
```
### Contenu des messages
* `PUT_FILE`: Message de type `PUT_FILE` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC2
    * `SIZE_NAME` : Taille du nom du fichier.
      * Ici, Integer
    * `NAME` : Nom du fichier.
      * Ici, Char*


* `ASERT_PUT_FILE`: Message de type `ASERT_PUT_FILE` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA3
   * `STATUS` : Statut de la requête.
     * 0x20 : Accepté
     * 0x41 : Refusé : Nom de fichier invalide
     * 0x43 : Refusé : Nom de fichier déjà pris
     * 0x50 : Refusé : Erreur internes du serveur
     * 0x52 : Refusé : Fichier trop volumineux
   * `SIZE_NAME` : Taille du nom du fichier.
     * Ici, Integer
   * `NAME` : Nom du fichier.
     * Ici, Char*


* `PUT_FILE_DATA`: Message de type `PUT_FILE_DATA` envoyé au serveur.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xC3
   * `SIZE_DATA` : Taille du fichier.
     * Ici, Integer
   * `DATA` : Fichier.
     * Ici, Char*


* `ASERT_PUT_FILE_DATA`: Message de type `ASERT_PUT_FILE_DATA` envoyé au client.
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
   |       GET_FILE        |
   |---------------------->|
   |       PUT_FILE        |
   |<----------------------|
```
### Contenu des messages
* `GET_FILE`: Message de type `GET_FILE` envoyé au serveur.
    * `CMD` : Commande indiquant le but de la requête.
      * Ici, 0xC4
    * `SIZE_NAME` : Taille du nom du fichier.
      * Ici, Integer
    * `NAME` : Nom du fichier.
      * Ici, Char*

    
* `PUT_FILE`: Message de type `PUT_FILE` envoyé au client.
   * `CMD` : Commande indiquant le but de la requête.
     * Ici, 0xA5
   * `STATUS`:
     * 0x20 : Accepté
     * 0x44 : Refusé : Fichier introuvable
     * 0x50 : Refusé : Erreur interne
   * `SIZE_DATA` : Taille du fichier.
     * Ici, Integer
   * `DATA` : Fichier.
     * Ici, Char*

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
### Commande serveur
* 0xA1 : LIST_PREFIX
* 0xA2 : PUT_FILE
* 0xA3 : ASERT_PUT_FILE
* 0xA4 : ASERT_PUT_FILE_DATA
* 0xA5 : PUT_FILE

### Commande client
* 0xC1 : LIST
* 0xC2 : PUT_FILE
* 0xC3 : PUT_FILE_DATA
* 0xC4 : GET_FILE

## Les Trames
@todo faire les trames
