#ifndef CHESS_HELP_H
#define CHESS_HELP_H


#include<stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name));


int getIntValue(char c);
char* replace(char *s, char ch, char *repl);
char *trimwhitespace(char *str);
int split(char *str, char c, char ***arr);
void freeArray(char** arrMul, int c);
void reduceSpaces(char *str);

#endif CHESS_HELP_H

