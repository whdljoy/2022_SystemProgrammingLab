#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct Thread_info{
    int index;
    int *m_row;
    int *result;
    int *v_col;
    int length;
}Thread_st;

void *calculate(void *arg) {
    int val = 0;
    Thread_st *new_arg=(Thread_st*)arg;
    for(int i=0; i< new_arg->length; i++){
        val+=(new_arg->m_row[i] * new_arg->v_col[i]);
    }
    new_arg->result[new_arg->index]=val;
    free(arg);
    pthread_exit(NULL);
}
int main() {
    int row,column;
    pthread_t *tid;
    long t;
    int **matrix=NULL;
    int *vec=NULL;
    int *rst=NULL;
    Thread_st *arg;
    arg = (Thread_st *)malloc(sizeof(Thread_st));
    scanf("%d %d",&row,&column);
    srand(time(NULL));
    matrix = (int **)malloc(sizeof(int*)*row);
    vec = (int *)malloc(sizeof(int)*column);
    rst = (int *)malloc(sizeof(int)*row);
    tid = (pthread_t *)malloc(sizeof(pthread_t)*row);
    for(int i = 0; i < row; i++){
        matrix[i]=(int *)malloc(sizeof(int) * column);
    }
    for(int a = 0; a < row; a++){
        rst[a]=0;
        for(int b = 0; b < column; b++){
            matrix[a][b]= rand() % 10;
        }
    }
    for (int f = 0; f < column; f++){
        vec[f]=rand() % 10;
    }
    printf(" *** Matrix ***\n");
    for(int a = 0; a < row; a++){
        for(int b = 0; b < column; b++){
            printf("[ %d ]  ",matrix[a][b]);
        }
        printf("\n");
    }
    printf(" *** Vector ***\n");
    for(int c = 0; c < column; c++){
        printf("[ %d ]\n",vec[c]);
    }    
    for (int v = 0; v < row; v++) {
        arg = (Thread_st *)malloc(sizeof(Thread_st));
        arg->length=column;
        arg->v_col=vec;
        arg->result=rst;
        arg->index=v;
        arg->m_row=matrix[v];
        if(pthread_create(&tid[v], NULL, calculate, (void*)arg)){
            printf("ERROR: pthread creation failed.\n");
            exit(1);
        }
    }
    for (t = 0; t < row; t++) {
        pthread_join(tid[t], NULL);
    }
    printf(" *** Result ***\n");
    for (int d = 0; d < row; d++) {
        printf("[ %d ]\n",rst[d]);
    }
    free(vec);
    free(rst);
    free(tid);
    for(int i=0; i< row; i++){
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}