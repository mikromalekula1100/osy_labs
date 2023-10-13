#include "../include/generating_cards.h"

const int count_cards = 52;
void* generating_cards(void* args) {
    
    printf("%ld \n", (*((struct Data*)(args))).round_count);
    

    long round_count = (*((struct Data*)(args))).round_count;

    pthread_mutex_lock(&(*((struct Data*)(args))).mutex);
    unsigned int seed = time(NULL);
    pthread_mutex_unlock(&(*((struct Data*)(args))).mutex);

    long first_value, second_value, number_matches = 0, number_failures = 0;

    for (long i = 0; i < round_count; i++)
    {   
        first_value = rand_r(&seed) % count_cards + 1;
        while((second_value = (rand_r(&seed) % count_cards + 1)) == first_value){continue;}
        
        if((abs(first_value - second_value) == 1) && ((first_value / 4) == (second_value / 4))){
            number_matches++;
        } 
        else{
            number_failures++;
        } 
    }
    pthread_mutex_lock(&(*((struct Data*)(args))).mutex);
            (*((struct Data*)(args))).sum[1] += number_failures;
            (*((struct Data*)(args))).sum[0] += number_matches;
    pthread_mutex_unlock(&(*((struct Data*)(args))).mutex);
    
    pthread_exit(0);
}