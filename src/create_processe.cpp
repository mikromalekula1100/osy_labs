#include "../include/create_processe.hpp"

int create_processe(){

    pid_t pid = fork();

    if(pid == -1){
        
        perror("Сreating a process");
        exit(-1);
    }
    return pid;
}
