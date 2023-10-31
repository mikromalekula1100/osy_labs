#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
    
        char buffer[1024];
        ssize_t n;
        while((n = read(0, buffer, sizeof(buffer)))>0){
            char upper_array[n];
            for (int i = 0; i < n; i++) {
                upper_array[i] = toupper((unsigned char)buffer[i]);
                }
            write(1, upper_array, n);
        }
        close(0);
        close(1);
}