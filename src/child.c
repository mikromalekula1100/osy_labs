#include "../include/create_processe.h"
#include "../include/child.h"


void reverse_string(char* string, int size_string){

    for(int i = 0; i < (size_string/2); ++i){

        char symbol_to_replace = string[i];
        string[i] = string[size_string-1 - i];
        string[size_string - 1 - i] = symbol_to_replace;
    }
}

void handle_sigterm() 
{
    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    sem_close(first_semafor);
    sem_close(parent_semafor);
    exit(0);
}

int main(int argc, char *argv[]){
    first_semafor =  sem_open(argv[1], 0);
    parent_semafor =  sem_open(argv[2], 0);

    const int max_buffer_size = 128;

    char string[max_buffer_size];

    struct stat sd;

    

    while(1){

        if (signal(SIGTERM, handle_sigterm) == SIG_ERR) {
            perror("Error while setting a signal handler");
            return EXIT_FAILURE;
        }

        sem_wait(first_semafor);

        if(fstat(STDIN_FILENO, &sd) == -1){
            perror("could not get file size. \n");
        }
    
        char* mp = mmap(NULL, sd.st_size, PROT_READ, MAP_SHARED, STDIN_FILENO, 0);

        for(int i = 0; i < sd.st_size; ++i){
            string[i] = mp[i];
        } 
       
        reverse_string(string, sd.st_size - 1);

        if(write(STDOUT_FILENO, string, sd.st_size) == -1){
            perror("write");
        }
    
        sem_post(parent_semafor);
        
    }
    
    
}