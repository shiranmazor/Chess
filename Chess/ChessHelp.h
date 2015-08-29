#pragma once
#include<stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name));
#define malloc(x) myMalloc(x)
#define free(x) myFree(x)
#define calloc(x,y) myCalloc(x,y)
#define realloc(x,y) myRealloc(x,y)

//functions declaration:
int getIntValue(char c);
char* replace(char *s, char ch, char *repl);
char *trimwhitespace(char *str);
int split(char *str, char c, char ***arr);
void freeArray(char** arrMul, int c);
void reduceSpaces(char *str);