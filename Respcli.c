#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void *encode(char *cli_input){
   char *res = (char*)(malloc(1000*sizeof(char)));
   char **pos = (char**)(malloc(sizeof(char*)));

   char *aux = strtok_r(cli_input," ",pos); 
   char *temp = (char*)(malloc(sizeof(char)));
  
   strcpy(res,"*3\r\n");
   
   int i = 0;
   while(aux != NULL){
    sprintf(temp,"$3\r\n%s\r\n",aux);
    strcat(res,temp);
    aux = strtok_r(NULL," ",pos);
    i++;
   }

   return res;
}

void *initSocketClientAddr(struct sockaddr_in *addr){
   addr->sin_family = AF_INET;
   addr->sin_port = htons(6379);
   inet_pton(AF_INET,"192.168.2.57",&(addr->sin_addr));
}

void main(){

  

   int client_fd = socket(AF_INET,SOCK_STREAM,0);
   if(client_fd == -1){
   perror("Client socket creation failed \n");
   exit(1);
   }

   //IP structure initialization for client socket connection
   struct sockaddr_in *client_addr = (struct sockaddr_in*)(malloc(sizeof(struct sockaddr_in)));
   initSocketClientAddr(client_addr);

   int client_con_fd = connect(client_fd,(struct sockaddr*)client_addr,sizeof(struct sockaddr_in));
   
   if(client_con_fd == -1){
      perror("Error connecting \n");
      exit(1);
   }
   
 
  //char *r = encode(test);
  char *test = (char*)(malloc(1000*sizeof(char))); 
  char *buf = (char*)(malloc(1000*sizeof(char)));

  while(1){
   printf("Give me your command \n");
   fgets(test,1000,stdin);
   test[strlen(test)-1] = '\0';
   char *r = encode(test);

   send(client_fd,r,strlen(r),0);
   
   recv(client_fd,buf,1000,0);
   printf("%s \n",buf);

  } 
     
}


   /*
   printf("encoded: ");
   int i = 0;
   while(i < strlen(r)){
      if(r[i] == '\r')
      printf("\\r");
      else if(r[i] == '\n')
           printf("\\n");
           else printf("%c",r[i]);

      i++;
   }
   printf("\n");
   */