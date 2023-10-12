#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> //для рандома
#include <sys/time.h>
#include <math.h>


pthread_mutex_t mutex;
long sum1 = 0;
long sum2 = 0;

// void* increment_function_plus_200(void *args) {
//     for (int i=0; i<100; i++)
//     {
//         pthread_mutex_lock(&mutex);
//         sum = sum + 2;
//         pthread_mutex_unlock(&mutex);
//     }
//     pthread_exit(0);
// }

int main() {
    struct timeval start_time, end_time;
    long seconds, microseconds;
    double elapsed_time;
    gettimeofday(&start_time, NULL); 


    for(int i = 0; i < 80000000; ++i){
    int value1, value2;
    value1 = rand() % 54 + 1;
    while((value2 = (rand() % 54 + 1)) == value1){continue;}
    if(abs(value1 - value2) == 1) sum1 ++;
    else sum2 ++;
    }
    
    
    printf("%ld %ld \n", sum1, sum2);

    gettimeofday(&end_time, NULL); 
    seconds = end_time.tv_sec - start_time.tv_sec;
    microseconds = end_time.tv_usec - start_time.tv_usec;
    elapsed_time = seconds + (microseconds / 1000000.0);
    printf("Время выполнения программы: %f секунд\n", elapsed_time);
    return 0;
}