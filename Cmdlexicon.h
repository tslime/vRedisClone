#ifndef CMDLEXICON_H
#define CMDLEXICON_H


#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<ctype.h>

#include "HTable.h"

typedef struct CMDNode{
    int num_args;
    char *command;
    void (*cmd_func)(HTable **t,void *cmd_arg);
}CMDNode;

typedef struct CMDTable{
    int lexicon_size;
    CMDNode *entries[];
}CMDTable;

void CMDLexicon(CMDTable **lex,int l_size);
void printLexicon(CMDTable *l);


#endif