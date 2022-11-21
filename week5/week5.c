#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int main(){
    while(1){
      char *cmd;
      size_t size;
      char *arg[100];
      int i = 0;
      getline(&cmd,&size,stdin);
      cmd[strlen(cmd)-1] = '\0';
      char *write=strchr(cmd,'>');
      char *read=strchr(cmd,'<');
      char *ptr = strtok(cmd," ");
      while (ptr != NULL) {
        arg[i++] = ptr;
        ptr = strtok(NULL," ");
      }
      arg[i] = NULL;
      char path[100];
      sprintf(path, "/bin/%s", arg[0]);
      if(strcmp(arg[0],"quit") == 0){
        exit(0);
      }
      else if(fork()==0){
        execv(path,arg);
      }
  }
  wait(NULL);
  exit(0); 
}

