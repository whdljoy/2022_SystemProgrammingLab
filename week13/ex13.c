#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
int point_in_circle,point_thread;
pthread_mutex_t mtx= PTHREAD_MUTEX_INITIALIZER;

void *calculate(){
    int point_in_thread = 0;
    unsigned int rand_seed = rand();
    for (int i = 0; i < point_thread; i++) {
        double x = rand_r(&rand_seed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_seed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        printf("%d %d",x,y);
        if ((x * x + y * y) <= 1) {
            point_in_thread++;
        }
    }
    pthread_mutex_lock(&mtx);
    point_in_circle+= point_in_thread;
    pthread_mutex_unlock(&mtx);
}
int main(){


    int num_thread, num_points;
    srand(time(NULL));
    printf("the number of threads? : ");
    scanf("%d",&num_thread);
    printf("the number random points? : ");
    scanf("%d",&point_thread);
    pthread_t *threads = (pthread_t *)malloc(num_thread * sizeof(pthread_t));\
    for (int i = 0; i < num_thread; i++) {
        pthread_create(&threads[i], NULL, calculate, (void *) NULL);
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mtx);
    free(threads);
    double pi = 4 * ((double)(point_in_circle)/(double)(num_thread*point_thread));
	printf("PI is  : %f\n", pi);    
}