#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> //для рандома
#include <sys/time.h>
#include <math.h>


// pthread_mutex_t mutex;
int sum1 = 0;
int sum2 = 0;

void* increment_function_plus_200(void *args) {
    int value1, value2;
    for (int i=0; i<125000000; i++)
    {   
        value1 = rand() % 54 + 1;
        while((value2 = (rand() % 54 + 1)) == value1){continue;}
        
        if(abs(value1 - value2) == 1){
            pthread_mutex_lock(&mutex);
            sum1 ++;
            pthread_mutex_unlock(&mutex);
        } 
        else{
            pthread_mutex_lock(&mutex);
            sum2 ++;
            pthread_mutex_unlock(&mutex);
        } 
        
    }
    pthread_exit(0);
}

int main() {
     struct timeval start_time, end_time;
    long seconds, microseconds;
    double elapsed_time;
    gettimeofday(&start_time, NULL); 

    const int threadCount = 8;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[threadCount];

    for (int i=0; i<threadCount; i++)
    {
        pthread_create(&tid[i], NULL, increment_function_plus_200, NULL);
    }
    for (int i=0; i<threadCount; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    printf("%d %d \n", sum1, sum2); 

    gettimeofday(&end_time, NULL); 
    seconds = end_time.tv_sec - start_time.tv_sec;
    microseconds = end_time.tv_usec - start_time.tv_usec;
    elapsed_time = seconds + (microseconds / 1000000.0);
    printf("Время выполнения программы: %f секунд\n", elapsed_time);
    return 0;
}