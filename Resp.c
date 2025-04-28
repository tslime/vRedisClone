#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<ctype.h>
#include<stdbool.h>


typedef struct Cmd{
int narg;
bool flag;
char **args;
char *err;
}Cmd;



Cmd *redis_parser(char *input){

    Cmd *pr = (Cmd*)(malloc(sizeof(Cmd)));
    pr->narg = 0;
    pr->flag = false;
    pr->err = (char*)(malloc(10000*sizeof(char)));
    pr->args = (char**)(malloc(1000*sizeof(char*)));//"*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n"
    int temp;

    if( input[0] != '*' || !isdigit(input[1])){
        pr->flag = true;
        if(input[0] != '*')
        sprintf(pr->err,"-ERR: expected * at position %d instead found %c",0,input[0]);
        else sprintf(pr->err,"-ERR: expected integer at position %d instead found %c",1,input[1]);
    }else{
        

        pr->narg = input[1] - '0';

        int i = 2;
        if(input[i] != '\r' || input[i+1] != '\n'){
            
            pr->flag = true;
            printf("test \n");
            if(input[i] != '\r')
            sprintf(pr->err,"-ERR: expected \r at position %d instead found %c",i,input[i]);
            else sprintf(pr->err,"-ERR: expected \n at position %d instead found %c",i+1,input[i+1]);
            }

        
        i+=2;
        int j = 0;
        while(i < strlen(input) && !(pr->flag)){

            if(input[i] != '$' || !isdigit(input[i+1]) || input[i+2] != '\r' || input[i+3] != '\n'){
                pr->flag = true;
                if(input[i] != '$')
                sprintf(pr->err,"-ERR: expected $ at position %d instead found %c",i,input[i]);
                else if(!isdigit(input[i+1]))
                     sprintf(pr->err,"-ERR: expected integer at position %d instead found %c",i+1,input[i+1]);
                    else if(input[i+2] != '\r')
                         sprintf(pr->err,"-ERR: expected \r at position %d instead found %c",i+2,input[i+2]);
                         else sprintf(pr->err,"-ERR: expected \n at position %d instead found %c",i+3,input[i+3]);

            }else {
                   temp = input[i+1] - '0';
                   i+=4;
                 }


            if(!(pr->flag)){
                char *aux = (char*)(malloc(temp+1*sizeof(aux)));
                memcpy(aux,&input[i],temp);
                aux[temp+1] = '\0';
                pr->args[j] = aux;
                j++;

                i = i+temp;
            }
            
            if(input[i] != '\r' || input[i+1] != '\n'){
            pr->flag = true;
            if(input[i] != '\r')
            sprintf(pr->err,"-ERR: expected \r at position %d instead found %c",i,input[i]);
            else sprintf(pr->err,"-ERR: expected \n at position %d instead found %c",i+1,input[i+1]);
            }else i+=2;
        }
                  
    }

    return pr;

}
    

   

void main(){

 

   char *test = (char*)(malloc(1000*sizeof(char))); 
  
   //test = "*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n";


   printf("Give me your string \n");
   fgets(test,1000,stdin);
   char *res = (char*)(malloc(1000*sizeof(char)));
   char **pos = (char**)(malloc(sizeof(char*)));

   char *aux = strtok_r(test," ",pos); 
   char *temp = (char*)(malloc(sizeof(char)));
  
   strcpy(res,"*3\r\n");
   
   int i = 0;
   while(aux != NULL){
    sprintf(temp,"$3\r\n%s\r\n",aux);
    strcat(res,temp);
    aux = strtok_r(NULL," ",pos);
    i++;
   }

   printf("\n");

   Cmd *s = redis_parser(res);
   printf("Your full command is: \n");
   int x;
   for(x=0;x<s->narg;x++)
   printf("%s ",s->args[x]);
   
   printf("\n");
    
}