#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <math.h>



void* generating_cards(void* args);

struct Data
    {   
        pthread_mutex_t mutex;
        long round_count;
        long sum[2];
    };