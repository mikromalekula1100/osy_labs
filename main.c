#include "./include/generating_cards.h"




int main() {
    
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    struct Data data;
    data.sum[0] = 0;
    data.sum[1] = 0;
    data.mutex = mutex;

    int thread_count;
    long round_count;

    printf("Введите количество потоков, используемое для вычислений.");
    scanf("%d", &thread_count);

    printf("Введите количество раундов.");
    scanf("%ld", &round_count);

    data.round_count = round_count / thread_count;
    

    pthread_t pthread_id[thread_count];


    struct timeval start_time, end_time;
    long seconds, microseconds;
    double elapsed_time;
    gettimeofday(&start_time, NULL); 
    


    for (int i = 0; i < thread_count; ++i)
    {   
        pthread_create(&pthread_id[i], NULL, generating_cards, (void*)(&data));
    }

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_join(pthread_id[i], NULL);
    }
    
    printf("%f \n", (double)data.sum[0] / data.sum[1]); 

    gettimeofday(&end_time, NULL); 
    seconds = end_time.tv_sec - start_time.tv_sec;
    microseconds = end_time.tv_usec - start_time.tv_usec;
    elapsed_time = seconds + (microseconds / 1000000.0);
    printf("Время выполнения программы: %f секунд\n", elapsed_time);
    return 0;
}