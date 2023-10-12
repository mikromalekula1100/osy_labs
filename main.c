#include "generating_cards.h"



int main() {
    struct timeval start_time, end_time;
    long seconds, microseconds;
    double elapsed_time;
    gettimeofday(&start_time, NULL); 

    const int threadCount = 8;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[threadCount];
    int idpthread[threadCount];
    for (int i=0; i<threadCount; i++)
    {   
        idpthread[i] = i+1;
        pthread_create(&tid[i], NULL, increment_function_plus_200, (void*)&idpthread[i]);
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