#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<ctype.h>
#include<stdbool.h>


#include "Resp.h"


void errFunc(Cmd *c,char *input,int pos,int chunk_pos){

    if(pos == 0){

    if(input[0] != '*'){
        c->flag = true;
        sprintf(c->err,"-ERR: expected * at position %d instead found %c",0,input[0]);
    }else if(!isdigit(input[1])){
           c->flag = true;
           sprintf(c->err,"-ERR: expected integer at position %d instead found %c",1,input[1]);
        }

    }
    
    if(pos == 2){
        if(input[pos] != '\r'){
                c->flag = true;
                sprintf(c->err,"-ERR: expected \\r at position %d instead found %c",pos,input[pos]);
            }else if(input[pos+1] != '\n'){
                   c->flag = true;
                   sprintf(c->err,"-ERR: expected \\n at position %d instead found %c",pos+1,input[pos+1]);
            }
    }
        
        
    if(pos > 2){

            if(chunk_pos == 0){

                if(input[pos] != '$'){
                    c->flag = true;
                    sprintf(c->err,"-ERR: expected $ at position %d instead found %c",pos,input[pos]);
                }else if(!isdigit(input[pos+1])){
                        c->flag = true;
                        sprintf(c->err,"-ERR: expected integer at position %d instead found %c",pos+1,input[pos+1]);
                      }else if(input[pos+2] != '\r'){
                            c->flag = true;
                            sprintf(c->err,"-ERR: expected \r at position %d instead found %c",pos+2,input[pos+2]);
                        }else if(input[pos+3] != '\n'){
                              c->flag = true;
                              sprintf(c->err,"-ERR: expected \n at position %d instead found %c",pos+3,input[pos+3]);
                            }

            }else{

                if(pos >= strlen(input)){
                   c->flag = true; 
                   sprintf(c->err,"-ERR: missing both \\r and \\n");
                }else if(input[pos] != '\r'){
                c->flag = true;
                sprintf(c->err,"-ERR: expected \\r at position %d instead found %c",pos,input[pos]);
                }else if(input[pos+1] != '\n'){
                      c->flag = true;
                      sprintf(c->err,"-ERR: expected \\n at position %d instead found %c",pos+1,input[pos+1]);
              }
            }

        }
    
}
    



Cmd *redis_parser(char *input){

    Cmd *pr = (Cmd*)(malloc(sizeof(Cmd)));
    pr->narg = 0;
    pr->size = 0;
    pr->flag = false;
    pr->err = (char*)(malloc(10000*sizeof(char)));
    pr->args = (char**)(malloc(1000*sizeof(char*)));//"*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n"
    int temp;

    errFunc(pr,input,0,0); //checkpoint 1

    if(!(pr->flag))
    {
        pr->narg = input[1] - '0';

        int i = 2;
        errFunc(pr,input,i,0); //checkpoint 2

        i+=2;
        while(i < strlen(input) && !(pr->flag)){

            errFunc(pr,input,i,0);  //checkpoint 3
            if(!(pr->flag)){
                temp = input[i+1] - '0';
                i+=4;

                char *aux = (char*)(malloc(temp+1*sizeof(aux)));
                memcpy(aux,&input[i],temp);
                aux[temp+1] = '\0';
                pr->args[pr->size] = aux;
                pr->size++;

                i = i+temp;
                
                errFunc(pr,input,i,1); //checkpoint 4
                i+=2;
            }
        }
    }

    return pr;
}
    
   
/*
void main(){

 

   char *test = (char*)(malloc(1000*sizeof(char))); 
  
   test = "*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n";

   printf("\n");

   Cmd *s = redis_parser(test);
   
 
   if(s->flag)
   printf(" %s ",s->err);
   else{
   printf("Your full command is: \n");
   int x;
   for(x=0;x<s->narg;x++)
   printf("%s ",s->args[x]);
    
   }
   printf("\n");
    
}*/