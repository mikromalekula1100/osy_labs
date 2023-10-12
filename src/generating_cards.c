#include "../include/generating_cards.h"

void* generating_cards(void* args) {

    pthread_mutex_lock(&mutex);
    unsigned int seed = time(NULL);
    pthread_mutex_unlock(&mutex);

    int value1, value2, Sum1 = 0, Sum2 = 0;
    int number = *((int*)args);
    printf("Я поток номер %d\n", number);
    for (int i=0; i<10000000; i++)
    {   
        value1 = rand_r(&seed) % 54 + 1;
        while((value2 = (rand_r(&seed) % 54 + 1)) == value1){continue;}
        
        if(abs(value1 - value2) == 1){
            Sum1++;
        } 
        else{
            Sum2++;
        } 
    }
    pthread_mutex_lock(&mutex);
            sum2 += Sum2;
            sum1 += Sum1;
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}