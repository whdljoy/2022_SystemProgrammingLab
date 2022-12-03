#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MAX_CLIENTS 1024
#define MAX_SEATS 256
typedef struct _query {
    int user;
    int action;
    int data;
} query;
typedef struct{
  int login;
	int userId;
  int passCode;
  int reserved;
} user;


pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int cli_count;
int clnt_socks[MAX_CLIENTS]; 
user users[MAX_CLIENTS];
int seats[MAX_SEATS];
int clients[MAX_CLIENTS];

void *handle_client(void *args);
int check_login(query *q);
int login(query *q);
int reserve(query *q);
int check_reserve(query *q);
int log_out(query *q);
int cancel_reserve(query *q);
int isFull();

int isFull(){
  int result = 1;
  for(int i= 0; i<MAX_SEATS; i++){
    if(seats[i] == -1){
      result = -1;
    }
  }
  return result;
}

void *handle_client(void *args){
  int clint_sock=*((int*)args);
  query *q=(query *)malloc(sizeof(query));
  int receive,result;
  while((receive=recv(clint_sock, q, sizeof(query), 0))>0){
    if(q->action ==0 && q->user==0 && q->data==0){
      send(clint_sock,seats,sizeof(seats),0);
    }
    else if (q->action < 1 || q->action > 5){
      result = -1;
      send(clint_sock,&result,sizeof(result),0);
    }
    else if(q->user < 0 || q->user > 1023){
      result = -1;
      send(clint_sock,&result,sizeof(result),0);
    }
    if(q->action == 1){
      result= login(q);
    }
    else{
      result = check_login(q);
    }
    send(clint_sock,&result,sizeof(result),0);

    pthread_mutex_lock(&clients_mutex);
    if (isFull() == 1){
      for(int i = 0; i<cli_count; i++){
        int end=-1;
        send(clients[i],&end,sizeof(end),0);
        close(clients[i]);
      }
      exit(1);
    }
    pthread_mutex_unlock(&clients_mutex);
  }
  
  pthread_mutex_lock(&clients_mutex);
  for(int i=0; i<cli_count; i++)   {
    if(clint_sock == clients[i]){
      while(i++ < cli_count-1){
        clients[i]=clients[i+1];
      }
      break;
    }
  }
  cli_count--;
  pthread_mutex_unlock(&clients_mutex);
  free(q);
  close(clint_sock);
  pthread_exit(NULL);
}
int check_login(query *q){
  pthread_mutex_lock(&clients_mutex);
  if(users[q->user].login == 1 && users[q->user].userId == q->user){
    if(q-> action == 2){
      return reserve(q);
    }else if( q->action == 3){
      return check_reserve(q);
    }else if( q->action == 4){
      return cancel_reserve(q);
    }else if( q-> action ==5){
      return log_out(q);
    }
  }else{
    pthread_mutex_unlock(&clients_mutex);
    return -1;
  }
}
int login(query *q){
  pthread_mutex_lock(&clients_mutex);
  int result;
    if(users[q->user].passCode == -1){
      users[q->user].passCode = q->data;
      users[q->user].userId=q->user;
      users[q->user].login=1;
      result = 1;
    }else{
      if(users[q->user].passCode == q->data){
          users[q->user].userId=q->user;
          users[q->user].login=1;
          result = 1;
      }
      else{
        result = -1;
      }
  }
  pthread_mutex_unlock(&clients_mutex);
  return result;
}
int reserve(query *q){
  int result;
  if((seats[q->data]==-1) || (seats[q->data] == q->user)){
    if(users[q->user].reserved == -1 ){
      seats[q->data] = q->user;
      result = q->data;
      users[q->user].reserved = q->data;      
    }
    else{
      result = -1;
    }
  }else {
    result = -1;
  }
  pthread_mutex_unlock(&clients_mutex);
  return result;
}
int check_reserve(query *q){
  int result;
  if(seats[q->data]==-1 || seats[q->data] != q->user){
    result = -1;
  }else if (seats[q->data] == q->user){
    result = q->data;
  }
  pthread_mutex_unlock(&clients_mutex);
  return result;
}
int cancel_reserve(query *q){
  int result;
  if((seats[q->data]==-1 )|| (seats[q->data] != q->user)){
    result = -1;
  }else if (seats[q->data] == q->user){
    seats[q->data]=-1;
    users[q->user].reserved  = -1;
    result = q->data;
  }
  pthread_mutex_unlock(&clients_mutex);
  return result;
}
int log_out(query *q){
  int result;
  if(users[q->user].login ==1){
    users[q->user].login=-1;
    users[q->user].userId=-1;
    result = 1;
  }else{
    result = -1;
  }
  pthread_mutex_unlock(&clients_mutex);
  return result;
}

int main(int argc, char *argv[]){

	char *ip = "127.0.0.1";
  struct hostent *h;
	int port = atoi(argv[1]);
  int connFd,clientLen,serverSocket;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;
  if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket() failed.\n");
    exit(1);
  }
  for (int i=0; i<MAX_SEATS; i++){
    seats[i]=-1;
  }
  memset((char *)&serv_addr, 0, sizeof(serv_addr));
  memset(users,-1,sizeof(users));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
  if (bind(serverSocket, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    printf("bind() failed.\n");
    exit(2);
  }
  if (listen(serverSocket, 5) < 0) {
    printf("listen() failed.\n");
    exit(3);  
  }
  pthread_t tid;
  while(1){
    clientLen = sizeof(cli_addr);
		if((connFd = accept(serverSocket, (struct sockaddr*)&cli_addr, (socklen_t *)&clientLen))< 0){
      printf ("accept() failed.\n");
      continue;
    };
    pthread_mutex_lock(&clients_mutex);
    clients[cli_count++]=connFd;
    if(cli_count == (MAX_CLIENTS+1)){
      clients[cli_count-1]=0;
      printf("Max clients.\n");
      close(connFd);
    }
    pthread_mutex_unlock(&clients_mutex);  
    pthread_create(&tid, NULL, handle_client,(void *)&connFd);
    pthread_detach(tid);
  
  }
  return 0;
}