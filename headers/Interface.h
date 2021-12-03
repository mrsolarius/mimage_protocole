#define CHD "\u2554" 
#define CBD "\u255A" 
#define CHG "\u2557" 
#define CBG "\u255D" 
#define MH "\u2550" 
#define MV "\u2551" 
//sert de main de remplacement, attention c'est temporaire
void Affiche(int sockfd);
//affiche le menu principal du client
void menu();
//affiche la liste des fichiers présents sur le serveur
void liste(char ** nom, unsigned char taille);
//Permet de choisir le fichier à envoyer
char * envoie();
//permet de récupérer le nom du fichier qui doit être téléchargé (les fichiers sont identifiées par leurs noms)(il donne le numéro du nom dans les fait)
int recuperation(char ** nom,unsigned char taille);
//affiche des ligne de séparation
void ligne();
void curseur();