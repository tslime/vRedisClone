#include<stdio.h>
#include<malloc.h>
#include<string.h>


void main(){

   char *test = (char*)(malloc(1000*sizeof(char)));
   
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
    
}