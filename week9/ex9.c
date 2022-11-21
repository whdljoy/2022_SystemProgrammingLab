#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
struct msgbuf {
	long mtype;
	char mtext[260];
};

int main(){

    int p1_id, p2_id;
    printf("my id is\n");
    scanf("%d", &p1_id);
    struct tm* pTm;
    time_t t2;
    printf("who to send ?\n");
    scanf("%d", &p2_id);
    key_t k = ftok(".", p2_id);
    key_t k_2 = ftok(".", p1_id);
    int qid1,qid2;
    if ((qid2 = msgget(k_2, IPC_CREAT| 0660)) < 0) {
		    perror("msgget");
		    exit(1);
	}
    if ((qid1 = msgget(k, IPC_CREAT| 0660)) < 0) {
            perror("msgget");
            exit(1);
    }
    struct msgbuf *sbuf = malloc(sizeof(struct msgbuf));
    struct msgbuf *rbuf = malloc(sizeof(struct msgbuf));
    struct msgbuf *arkbuf = malloc(sizeof(struct msgbuf));
    struct msgbuf *tbuf = malloc(sizeof(struct msgbuf));                        
    int pid,pid2;
    while(pid=fork()==0){    
        if (msgrcv(qid2, (void *)rbuf, sizeof(struct msgbuf),1, 0) < 0) {
             perror("Recive Error");
            exit(1);
        }else{
            printf("Received: %s\n", rbuf->mtext);
            tbuf->mtype = 2;
            time(&t2);
            pTm =localtime(&t2);
            strcpy(tbuf->mtext,asctime(pTm));
            if (msgsnd(qid1, (void *)tbuf, sizeof(struct msgbuf), IPC_NOWAIT) < 0) {
                perror("sendArkError");
                exit(1);
            }              
        }
    }                                         
    while(pid2=fork()==0){
        if (msgrcv(qid2, (void *)arkbuf, sizeof(struct msgbuf),2, 0) < 0) {
            perror("ReceivedArkErro");
            exit(1);
        }else{
            printf("%d read message at: %s\n",p2_id, arkbuf->mtext);         
        }        
    }    
    while(1){ 
        read(STDIN_FILENO,sbuf->mtext,sizeof(sbuf->mtext));
        sbuf->mtype = 1;
        if (msgsnd(qid1, (void *)sbuf, sizeof(struct msgbuf), IPC_NOWAIT) < 0) {
            perror("SendErro");
            exit(1);
        }
    }
    free (sbuf);
    free (rbuf);
    free (tbuf); 
    free (arkbuf);    
    exit(0);
}