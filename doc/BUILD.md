# Makefile et build
Dans ce projet, nous avons décidé de respecter l'arborecence de fichiers suivante :
* `bin` pour les fichiers executables
* `debug` pour les fichiers executables de debug ainsi que les .obj de debug
* `doc` pour les fichiers de documentation
* `headers` pour les fichiers headers
* `obj` pour les fichiers objets
* `sources` pour les fichiers sources
* `tests` pour les fichiers de tests

Pour se faire, nous avons d'abord commencé par indiquer dans notre makefile les fichiers à build un à un.
Nous rendant bien compte que cela ne serait plus vraiment efficace pour la suite du projet, nous avons essyé de l'implémenter
nous même a l'aide des variables de configuration.

Or n'étant pas expert en makefile, nous avons donc décidé d'utiliser un template de makefile que nous avons modifié
afin d'y inclure nos headers et de permettre la compilation de nos tests unitaires.

Celui-ci dispose des commandes suivantes :
* `all` qui permet de build tous les fichiers du projet
* `test` qui permet de build les fichiers de test
* `debug` qui permet de build les fichiers du projet en mode debug
* `clean` qui permet de clean tous les fichiers obj et bin du projet

Nous avons aussi d'autres commandes annexes utilisées par les commandes précédente :
* `makedir` qui permet de créer les dossiers de build si ils n'existent pas
* `build-test` qui permet de build les fichiers de test sans créer les dossiers de build
* `distclean` qui permet de supprimer les dossiers de build et bin du projet

Par defaut nous appelons la commande `all` lorsque l'on exécute la commande `make`.

## Lancement du build
ATTENTION IL ET NECESAIRE DE LANCER LES EXECUTABLES DEPUIS LA RACINE DU PROJET !

Pour build et lancer le projet, il faut faire les commande suivante :
```shell
$ make
$ ./bin/mimage
````

Pour build et lancer les tests unitaire, il faut faire les commande suivante :
```shell
$ make test
$ ./bin/run_tests
```
