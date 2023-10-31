#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    const int max_size_buff = 1024;
    pid_t child1, child2;

    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child1 = fork();

    if (child1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    

    if (child1 == 0) {
        
        close(pipe1[1]); 
        close(pipe2[0]);  
        
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);

        execl("../build/child1", " ", NULL);

        perror("child1");
        return -1;
    }

    int pipe3[2];
    if (pipe(pipe3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child2 = fork();
    if (child2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {
        close(pipe1[1]);
        close(pipe1[0]);

        close(pipe2[1]);
        close(pipe3[0]);  

        dup2(pipe2[0], 0);
        dup2(pipe3[1], 1);
        execl("../build/child2", " ", NULL);
        
        perror("child2");
        return -1;
        
    }

    close(pipe1[0]);
    char input_buffer[max_size_buff];

    char kek[20] = "Enter a string: \n";
 
    write(1, kek, 20);

    int size;
    while((size = read(0, input_buffer, max_size_buff)) > 0){   
        write(pipe1[1], input_buffer, size);
    }
    close(pipe1[1]);
    
    char result_buffer[max_size_buff];
    ssize_t n;

    close(pipe3[1]);
    close(pipe2[1]); 
   
    char result[20] = "Result: \n";
    write(1, result, 20);
    while((n = read(pipe3[0], result_buffer, sizeof(result_buffer))) > 0){
        write(1, result_buffer, n);
    }
    
    close(pipe2[0]);
    
    close(pipe3[0]);
   

    return 0;
}