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

void print_hex(const unsigned char *s);

void printTitle(char *title);

void passTest(char *testFunction, char *msg, bool res);

void resume();
#endif // test_core_h