#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include "unistd.h"
#include "sys/wait.h"

int main(){
    onst int max_buffer_size = 50;

    printf("Введите названия файлов.\n");

    char file_name_first[max_buffer_size];
    
    scanf("%s", file_name_first);

    int first_file_descriptior = open(file_name_first,  O_CREAT | O_WRONLY, S_IRWXU);
    
    pid_t x = fork();

    if(x == 0){
        dup2(first_file_descriptior, 0);
        execl("../build/test2", NULL);
    }
}