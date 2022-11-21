#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAXLINE 80
int main (int argc, char *argv[]) {

    int n, listenfd, connfd, caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    char filename[MAXLINE];
    int readnum,fd,total;
    char file[]="File name : ";
    int port = atoi(argv[1]);
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }
    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }
    while (1) {
        total=0;
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr,(socklen_t *)&caddrlen)) < 0) {
            printf ("accept() failed.\n");
            continue;
        }
        readnum = read(connfd, filename, MAXLINE);
        filename[readnum-1]='\0';
        write(STDOUT_FILENO,file,sizeof(file));
        write(STDOUT_FILENO,filename,readnum);
        write(STDOUT_FILENO,"\n",1);
        if(strcmp(filename,"quit")==0){
            break;
        }
        char path[100];
        sprintf(path, "%s_copy",filename);
        fd=open(path, O_WRONLY | O_CREAT, 0755);
        while ((n = read(connfd, buf, MAXLINE)) > 0) {
            total += n;
            write(fd, buf, n);
        }
        printf ("got %d bytes from client.\n", total);
        close(fd);
    }
    close(connfd);
}