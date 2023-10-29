#include "../include/create_processe.h"


int main() {
    const int max_buffer_size = 50;

    printf("Введите названия файлов.\n");

    char file_name_first[max_buffer_size];
    
    scanf("%s", file_name_first);

    int first_file_descriptior = open(file_name_first,  O_CREAT | O_WRONLY, S_IRWXU);
    
    if(first_file_descriptior == -1){

        perror("Opening file number 1");
        return -1;  
    }

    int pipe_fisrt[2];

    int first_error = pipe(pipe_fisrt);

    if(first_error == -1){
        
        perror("Creature a pipe 1");
        return -1;
    }

    pid_t proccess_id = create_processe();
    
    if(proccess_id == 0){

        close(pipe_fisrt[1]);

        dup2(pipe_fisrt[0], STDIN_FILENO);
        dup2(first_file_descriptior, STDOUT_FILENO);

        execl("../build/child", " ", NULL);
        perror("Execl in child 1");
        return -1;
    }
    
    char file_name2[max_buffer_size];

    scanf("%s", file_name2);

    int second_file_descriptior = open(file_name2,  O_CREAT | O_WRONLY, S_IRWXU);

    if(second_file_descriptior == -1){

        perror("Opening file number 2"); 
        return -1;
    }

    close(pipe_fisrt[0]);

    int pipe_second[2];
    int second_error = pipe(pipe_second);

    if(second_error == -1){

        perror("Creature a pipe 2");
        return -1;
    }

    proccess_id = create_processe();

    if(proccess_id == 0){

        close(pipe_second[1]);

        dup2(pipe_second[0], STDIN_FILENO);
        dup2(second_file_descriptior, STDOUT_FILENO);

        execl("../build/child", " ", NULL);
        perror("Execl in child2");
        return -1;
    }

    close(pipe_second[0]);

// -----




    char string [max_buffer_size];
    
    int count;

    while((count = read(0, string, max_buffer_size))>0){
        
        if(count > 11){

            write(pipe_second[1], string, count); 
        }
        else{

            write(pipe_fisrt[1], string, count);
        }

    }

    close(first_file_descriptior);
    close(second_file_descriptior);
    return 0;
}