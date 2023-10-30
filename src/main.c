#include "../include/create_processe.h"


int main() {
    const int max_buffer_size = 128;

    printf("Введите названия файлов.\n");

    char file_name[max_buffer_size];
    
    scanf("%s", file_name);

    char semafor_name_third[] = "/semafor3";
    sem_t* third_semafor =  sem_open(semafor_name_third, O_CREAT, S_IRWXU, 0);

    int first_file_descriptior = open(file_name,  O_CREAT | O_RDWR, S_IRWXU);
    
    if(first_file_descriptior == -1){

        perror("Opening file number 1");
        return -1;  
    }

    char semafor_name_first[] = "/semafor1";
    sem_t* first_semafor =  sem_open(semafor_name_first, O_CREAT, S_IRWXU, 0);

    char mmap_file_name[] = "/mmap_file";
    int mmap_file_descriptior = shm_open(mmap_file_name, O_CREAT | O_RDWR, S_IRWXU);

    if (mmap_file_descriptior == -1) {
        perror("open mmap_file_descriptior");
        return -1;
        }

    pid_t child_pid_first = create_processe();
    
    if(child_pid_first == 0){

        dup2(mmap_file_descriptior, STDIN_FILENO);
        dup2(first_file_descriptior, STDOUT_FILENO);

        execl("../build/child", " ", semafor_name_first, semafor_name_third, NULL);
        perror("Execl in child 1");
        return -1;
    }
    
    scanf("%s", file_name);

    int second_file_descriptior = open(file_name,  O_CREAT | O_RDWR, S_IRWXU);

    if(second_file_descriptior == -1){

        perror("Opening file number 2"); 
        return -1;
    }

    char semafor_name_second[] = "/semafor2";
    sem_t* second_semafor =  sem_open(semafor_name_second, O_CREAT, S_IRWXU, 0);

    pid_t child_pid_second = create_processe();

    if(child_pid_second == 0){

        dup2(mmap_file_descriptior, STDIN_FILENO);
        dup2(second_file_descriptior, STDOUT_FILENO);

        execl("../build/child", " ", semafor_name_second, semafor_name_third, NULL);
        perror("Execl in child2");
        return -1;
    }

    char string [max_buffer_size];
    
    int count;

    while((count = read(0, string, max_buffer_size)) > 0){

        if (ftruncate(mmap_file_descriptior, count) == -1) {
            perror("Could not set size of mfile.txt");
            return 1;
            }

        char* mp = mmap(NULL, count, PROT_WRITE, MAP_SHARED, mmap_file_descriptior, 0);
            if (mp == MAP_FAILED) {
            perror("mmap");
            return -1;
            }   
        for(int i = 0; i < count; ++i){
            mp[i] = string[i];
        }

        if(count > 11){ 
            sem_post(second_semafor);
        }
        else{
            sem_post(first_semafor);
        }
        
        sem_wait(third_semafor);
    }
    if (kill(child_pid_first, SIGTERM) == -1) {
        perror("Ошибка при отправке сигнала");
    }
    if (kill(child_pid_second, SIGTERM) == -1) {
        perror("Ошибка при отправке сигнала");
    }

    unlink(mmap_file_name);

    sem_close(first_semafor);
    sem_close(second_semafor);
    sem_close(third_semafor);

    sem_unlink(semafor_name_first);
    sem_unlink(semafor_name_second);
    sem_unlink(semafor_name_third);
    
    close(first_file_descriptior);
    close(second_file_descriptior);
    return 0;
}