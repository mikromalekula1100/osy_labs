#include "../include/create_processe.h"

int create_processe(){

    pid_t pid = fork();

    if(pid == -1){
        
        perror("Сreating a process");
        exit(-1);
    }
    return pid;
}
