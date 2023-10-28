#include "stdio.h"
#include "unistd.h"

//для работы с файлами
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//работа с mmap
#include <sys/mman.h>



int main(){

    // int max_buf_size = 4096;

    // char str [max_buf_size];
    
    // int mfd = open("../mfile.txt", O_RDWR | O_CREAT, S_IRWXU);

    // if (mfd == -1) {
    //     perror("Could not open mfile.txt");
    //     return 1;
    // }

    // if (ftruncate(mfd, max_buf_size) == -1) {
    //     perror("Could not set size of mfile.txt");
    //     return 1;
    // }
    
    // pid_t id = fork();

    // if(id == 0){

    //     int fd_out = open("../output.txt", O_WRONLY | O_CREAT, S_IRWXU);

        
    //     struct stat sd;

    //     sleep(3);

    //     if(fstat(mfd, &sd) == -1){
    //             perror("could not get file size. \n");
    //         }

    //     size_t file_size = sd.st_size;

    //     char* mfd_in = mmap(NULL, file_size, PROT_READ, MAP_SHARED, mfd, 0);

    //     for(size_t i = 0; i < file_size; ++i){
    //                 str[i] = mfd_in[i];
    //             }
    //     write(fd_out, str, file_size);
    //     munmap(mfd_in, max_buf_size);
    //     close(mfd); // закрыть mfd
    //     close(fd_out);
        
    // }
    // else{
    //     int fd_in = open("../input.txt", O_RDONLY | O_CREAT, S_IRWXU);

    //     int count = read(fd_in, str, max_buf_size);

    //     if (ftruncate(mfd, count) == -1) {
    //         perror("Could not set size of mfile.txt");
    //         return 1;
    //         }
        
    //     char* mfd_out = mmap(NULL, count, PROT_WRITE, MAP_SHARED, mfd, 0);
        
    //     for(int i = 0; i < count; ++i){
    //                 mfd_out[i] = str[i];
    //             }
    //     munmap(mfd_out, max_buf_size);
    //     close(mfd); // закрыть mfd
    //     close(fd_in);
    // }
    char kek[9] = "/semafor1";
    for(int i = 0; i < sizeof(kek);++i)
        printf("%c", kek[i]);
    
}