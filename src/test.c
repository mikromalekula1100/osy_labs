#include "stdio.h"
#include "unistd.h"

//для работы с файлами
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//работа с mmap
#include <sys/mman.h>



int main(){

    int max_buf_size = 4096;

    char str [max_buf_size];
    
    int mfd = open("../mfile.txt", O_RDWR);
    
    pid_t id = fork();

    if(id == 0){

        int fd_out = open("../output.txt", O_WRONLY);

        struct stat sd;

        sleep(3);

        if(fstat(mfd, &sd) == -1){
                perror("could not get file size. \n");
            }

        size_t file_size = sd.st_size;

        char* mfd_in = mmap(NULL, max_buf_size, PROT_READ, MAP_SHARED, mfd, 0);

        for(size_t i = 0; i < file_size; ++i){
                    str[i] = mfd_in[i];
                }
        write(fd_out, str, file_size);
        
    }
    else{
        int fd_in = open("../input.txt", O_RDONLY);

        int count = read(fd_in, str, max_buf_size);
        
        char* mfd_out = mmap(NULL, max_buf_size, PROT_WRITE, MAP_SHARED, mfd, 0);
        
        for(int i = 0; i < count; ++i){
                    mfd_out[i] = str[i];
                }
    }
    
    
}