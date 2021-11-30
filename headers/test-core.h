#ifndef test_core_h
#define test_core_h
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

/**
 * Fonction d'affichage en hexa d'un tableau de char
 * @param tab tableau de char
 * @param size taille du tableau
 */
void print_hex(const unsigned char *s);

/**
 * Fonction permettant d'ecrire un titre dans la console
 * @param title titre à ecrire
 */
void printTitle(char *title);

/**
 * Fonction permettant de passer un tests
 * @param title titre du tests
 * @param test fonction de tests
 * @param res valeur attendue
 */
void passTest(char *testFunction, char *msg, bool res);

/**
 * Fonction qui permet de résumer le nombre de tests passés et échoués
 */
void resume();
#endif // test_core_h
