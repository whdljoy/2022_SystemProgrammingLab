#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
void handler (int sig) {
    for(int i=0; i<5;i++){
        printf("BEEP\n");
        sleep(1);
    }
    printf("I'm Alive!");
    exit(0);
}

int main(void) {
    int i,child_status;
    pid_t pid;
    signal (SIGINT, handler);
    while(1);
    return 0;
}