#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void reverseArray(char* array, int size) {
  char temp;
  for (int i = 0; i < size / 2; i++) {
    temp = array[i];
    array[i] = array[(size - 1) - i];
    array[(size - 1) - i] = temp;
  }
}
int main (int argc, char* argv[]){
  char numtxt[]="_num.txt";
  int fd,fd2,len = 0,check,count,index=1;
  char buffer[512];
  for (int i = 0; argv[1][i] != '\0'; i++) len++;

  char filename[len];
  strcpy(filename,argv[1]);
  char *newfile=strtok(filename,".");
  strcat(newfile,numtxt);

  if((fd = open(argv[1], O_RDONLY, 0755)) < 0 || (fd2= open(newfile, O_CREAT | O_WRONLY | O_RDONLY,0755)) < 0 ){
    perror("open");
    exit(1);
  }
  else{
    if(index==1){
      buffer[0]=(char)(index+48);
      buffer[1]=32;
      buffer[2]=124;
      buffer[3]=32;
      write(fd2,buffer,4);
      index++;
    }
    while(count=read(fd,buffer,1) > 0){
      if(buffer[0]=='\n'){
        check=write(fd2,buffer,count);
        if(check!=count){
          perror("write");
          exit(1);
        }
        int total=index;
        int leng=0;
        while(1){
          buffer[leng]=(char)(total%10+48);
          if(total/10 == 0){
            break;
          }
          total=total/10;
          leng++;
        }
        reverseArray(buffer,leng+1);
        buffer[leng+1]=32;
        buffer[leng+2]=124;
        buffer[leng+3]=32;
        write(fd2,buffer,leng+4);
        index++;
      }
      else{
        check=write(fd2, buffer, count);
        if(check!=count){
          perror("write");
          exit(1);
        }
      }
    }
  }
    close(fd);
    close(fd2);

}
