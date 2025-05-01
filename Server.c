#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<signal.h>






void initSocketServerAddr(struct sockaddr_in **s){
    *s = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    (*s)->sin_family = AF_INET;
    (*s)->sin_port = htons(6379);
    inet_pton(AF_INET,"192.168.2.57",&((*s)->sin_addr));
}

void main(){
    

    
    //Socket creation, TCP style
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0){
    perror("Server error \n");
    exit(1);
    }
    
    //Initialize server IP structure with a specific address so we can bind the socket to an address
    struct sockaddr_in *server_addr;
    initSocketServerAddr(&server_addr);
    int bind_result = bind(server_fd,(struct sockaddr*)server_addr,sizeof(struct sockaddr_in));
    if(bind_result < 0){
        perror("Error binding ip address \n");
        exit(1);
    }
        
    //Opens the connection to the socket in its specified address
    int server_l = listen(server_fd,5);
    if(server_l < 0){
    perror("Server failed listening \n");
    exit(1);
    }
        
        printf("Server listening at port 6379 \n");
        
        //creates events table
        int events_fd = epoll_create1(0);
        
        //Register server in the events table
        struct epoll_event *server_event = (struct epoll_event*)(malloc(sizeof(struct epoll_event)));
        server_event->events = EPOLLIN;
        server_event->data.fd = server_fd;
        epoll_ctl(events_fd,EPOLL_CTL_ADD,server_fd,server_event);

        //Array that holds events that are ready in the table and a buffer for clients as well as client fd
        struct epoll_event *events = (struct epoll_event*)(malloc(10*sizeof(struct epoll_event)));
        char *buffer = malloc(10000);
        int client_fd;

        while(true){
                int num_events = epoll_wait(events_fd,events,10,3);

                int i = 0;
                while(i < num_events){
                   
                   if(events[i].data.fd == server_fd){
                      struct sockaddr_in *client_addr = (struct sockaddr_in*)(malloc(sizeof(struct sockaddr_in)));
                      socklen_t *client_addr_len = (socklen_t*)(malloc(sizeof(socklen_t))); 
                      *client_addr_len = sizeof(struct sockaddr_in);

                      client_fd = accept(server_fd,(struct sockaddr*)client_addr,client_addr_len);
                      if(client_fd == -1){
                      perror("accepting client failed \n");
                      exit(1);
                      }
                      
                      //Accept successful, add client fd to events table
                      struct epoll_event *client_event = (struct epoll_event*)(malloc(sizeof(struct epoll_event)));
                      client_event->events = EPOLLIN;
                      client_event->data.fd = client_fd;
                      epoll_ctl(events_fd,EPOLL_CTL_ADD,client_fd,client_event);

                      printf("client connected \n");

                    }else{

                        memset(buffer,0,10000);
                        int r = read(events[i].data.fd,buffer,10000);
                        if(r <=0){
                        epoll_ctl(events_fd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                        close(events[i].data.fd);
                        }else{
                          printf("client %d says: %s \n",events[i].data.fd,buffer);
                          write(events[i].data.fd,"test from the other side\r\n",strlen("test from the other side\r\n"));
                          memset(buffer,0,10000);
                        }
                        
                      }

                    i++;
                   }
                   
                }

}










      /*fork based  
              pid_t pid = fork();

              if(pid == 0){
                printf("client connected \n");
        
                char *buffer = malloc(10000);

                while( read(client_fd,buffer,10000) > 0){
                printf("client %d says: %s \n",getpid(),buffer);
                write(client_fd,"test from the other side\r\n",strlen("test from the other side\r\n"));
                memset(buffer,0,10000);
                }
                
                sleep(1); 
                close(client_fd);            
              }else close(client_fd);
              */