
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

# Gestion de projet
Afin d'améliorer notre productivité mais aussi pour paralléliser le travail, nous avons décidé d'utiliser GitHubProject,
une fonctionnalité liée à notre répository GitHub.

Le pojet s'apparente à un tableau de kanban avec les colonnes suivantes :
* `To Do` : Les taches qui n'ont pas encore été développées
* `In Progress` : Les taches qui ont été testées mais qui n'ont pas encore été validées
* `Done` : Les taches qui ont été validées

Sur chacune des colonnes, nous avions des *issues* qui s'apparentent à des tickets. Tous les soirs nous ajoutions des tâches à faire pour le lendemain.
Puis, le lendemain matin, nous nous les assignions pour les développer.
Enfin nous nous créions une branche pour effecuter les modification imposées par l'*issue*.

Au début du projet nous développions les tâches qui nous avaient été assignés seul. Mais après quelques heures à procéder de cette manière,nous nous sommes rendu compte que nous étions plus productif et attentif aux erreurs lorsque nous travaillions à deux.

Nous avons ainsi travailler par paire sur chaque *issue*.

Lorsque l'on avait terminé une *issue*, nous ouvrions alors une pull request afin de la valider par les membres du groupe ne travaillant pas sur l'*issue*.
Une fois leur retour mis en ligne sur Github, nous avions deux choix :
* nous effectuions les modifications demandées
* s'ils validaient la pull request, nous effectuions un merge