#include "../include/create_processe.h"


void reverse_string(char* string, int size_string){

    for(int i = 0; i < (size_string/2); ++i){

        char symbol_to_replace = string[i];
        string[i] = string[size_string-1 - i];
        string[size_string - 1 - i] = symbol_to_replace;
    }
}

int main(){
    const int max_buffer_size = 50;

    char string[50];

    int count;

    while((count = read(STDIN_FILENO, string, 50))>0){

        reverse_string(string, count-1);
        write(STDOUT_FILENO, string, count);
    }
    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    return 0;
}