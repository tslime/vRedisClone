#ifndef RESP_H
#define RESP_H

#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<ctype.h>
#include<stdbool.h>


typedef struct Cmd{
int narg;
bool flag;
int size;
char **args;
char *err;
}Cmd;


void errFunc(Cmd *c,char *input,int pos,int chunk_pos);

Cmd *redis_parser(char *input);


#endif