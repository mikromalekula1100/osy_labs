#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <math.h>

pthread_mutex_t mutex;
int sum1 = 0;
int sum2 = 0;

void* generating_cards(void* args);