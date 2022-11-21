#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
int main(){
    while(1){
      char *cmd;
      size_t size;
      pid_t pid,pid2;
      char *arg[100];
      char **opt1;
      char **opt2;
      int i = 0,fd[2];
      getline(&cmd,&size,stdin);
      cmd[strlen(cmd)-1] = '\0';
      char *ptr = strtok(cmd," ");
      while (ptr != NULL) {
        arg[i++] = ptr;
        ptr = strtok(NULL," ");
      }
      arg[i] = NULL;
      char path[100];
      sprintf(path, "/bin/%s", arg[0]);
      int rd = 0;
      int wrt =0;
      int pip = 0;
      for(int a =0; a<i; a ++ ){
          if(strcmp(arg[a],">") == 0){
            wrt= a;
            break;
          }
          else if(strcmp(arg[a],"<") == 0){
            rd = a;
            break;
          }
          else if(strcmp(arg[a],"|") == 0){
            pip= a;
            break;
          }            
      }      
      if(strcmp(arg[0],"quit") == 0){
        exit(0);
      } 
      else if(pip ==0) {
        if((pid = fork()) <0){
          exit(1);
        }   
        else if (pid ==0){
          if (rd != 0){
            opt1 = (char **)malloc(sizeof(char*)* i+1);
            for(int b = 0; b < rd; b++){
              opt1[b]=arg[b];
            }
            opt1[rd]=NULL;  
            int fd0 = open(arg[rd+1], O_RDONLY);
            dup2(fd0, STDIN_FILENO);
            close(fd0);
            execv(path,opt1);
          }    
          else if(wrt != 0) {
            opt1 = (char **)malloc(sizeof(char*)* i+1);
            for(int b = 0; b < wrt; b++){
              opt1[b]=arg[b];
            }
            opt1[wrt]=NULL; 
            int fd1= open(arg[wrt+1], O_CREAT | O_RDWR ,0755);
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            execv(path,opt1);         
          }
          else{
            execv(path,arg);
          }
        }
      }
      else {
        if(pipe(fd) <0){
            exit(1);
        }
        opt1 = (char **)malloc(sizeof(char*)* i+1);
        opt2 = (char **)malloc(sizeof(char*)* i+1);
        for(int b = 0; b < pip; b++){
          opt1[b]=arg[b];
        }
        for(int c =0; c <i-pip; c++ ){
          opt2[c]=arg[c+pip+1];
        }
        opt1[pip]=NULL;
        opt2[i-pip]=NULL;
        char path2[100];
        sprintf(path2, "/bin/%s", opt2[0]);                  
        pid = fork();
        if(pid == 0){
          dup2(fd[1], STDOUT_FILENO);
          close(fd[0]);
          close(fd[1]);
          execv(path,opt1); 
        }
        pid2=fork();
        if(pid2 ==0){
          dup2(fd[0], STDIN_FILENO);
          close(fd[0]);
          close(fd[1]);
          execv(path2,opt2);          
        }
      }
  }
  
  wait(NULL);
  exit(0); 
}

