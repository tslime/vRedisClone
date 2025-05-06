#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<ctype.h>

#include "Cmdlexicon.h"
#include "HTable.h"




void CMDLexicon(CMDTable **lex,int l_size){

    int code;
    *lex = (CMDTable*)(malloc(sizeof(CMDTable) + l_size*sizeof(CMDNode)));
    int i;
    for(i=0;i<l_size;i++){
        (*lex)->entries[i] = (CMDNode*)(malloc(sizeof(CMDNode)));
        (*lex)->entries[i]->num_args = 0;
        (*lex)->entries[i]->command = NULL;
    }

    (*lex)->lexicon_size = l_size;

    //SET
    CMDNode *set = (CMDNode*)(malloc(sizeof(CMDNode)));
    set->num_args = 3;
    set->command = "SET";
    set->cmd_func = ht_set;
    code = ht_code(l_size,"SET");
    (*lex)->entries[code] = set;


    //GET
    CMDNode *get = (CMDNode*)(malloc(sizeof(CMDNode)));
    get->num_args = 2;
    get->command = "GET";
    get->cmd_func = ht_set;
    code = ht_code(l_size,"GET");
    (*lex)->entries[code] = get;

    //DEL
    CMDNode *del = (CMDNode*)(malloc(sizeof(CMDNode)));
    del->num_args = 2;
    del->command = "DEL";
    del->cmd_func = ht_set;
    code = ht_code(l_size,"DEL");
    (*lex)->entries[code] = del;
    
    
}

void printLexicon(CMDTable *l){
    if(l->lexicon_size == 0)
    printf("The lexicon is empty");
    else{
        int i = 0;
        while(i < l->lexicon_size){
            if(l->entries[i]->command != NULL)
            printf("%s ",l->entries[i]->command);
            
            i++;
        }
    }

}

/*
void main(){

    

    CMDTable *lexicon;
    CMDLexicon(&lexicon,5);

    printf("The lexicon contains the following command: \n");
    printLexicon(lexicon);
    printf("\n");
    
}*/