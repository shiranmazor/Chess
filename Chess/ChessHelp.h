#ifndef CHESS_HELP_H
#define CHESS_HELP_H


#include<stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name));
#define max(a,b) a > b ?a : b;
#define min(a,b) a < b ?a : b;

int objectsInMemory;
void *myMalloc(size_t size);
void myFree(void  *memory);
void *myCalloc(size_t  Count, size_t size);
void *myRealloc(void  *memory, size_t newSize);


int getIntValue(char c);
char* replace(char *s, char ch, char *repl);
char *trimwhitespace(char *str);
int split(char *str, char c, char ***arr);
void freeArray(char** arrMul, int c);
void reduceSpaces(char *str);
int StartsWith(const char *a, const char *b);
char *str_replace(char *orig, char *rep, char *with);
int str_cut(char *str, int begin, int len);


#endif CHESS_HELP_H

