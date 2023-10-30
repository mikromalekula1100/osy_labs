#include "../include/create_processe.h"
#include "../include/constants.h"

int main() {

    printf("Введите названия файлов.\n");
    
    char file_name[max_buffer_size];
    
    char* list_semafore_name[] = {"/semafor1", "/semafor2", "/semafor3"};

    sem_t* list_semafore[count_semafore];

    for(int i = 0; i < count_semafore; ++i){
        list_semafore[i] = sem_open(list_semafore_name[i], O_CREAT, S_IRWXU, 0);
    }

    int list_file_description[count_processes];

    for(int i = 0; i < count_processes; ++i){
        scanf("%s", file_name);
        if((list_file_description[i] = open(file_name,  O_CREAT | O_RDWR, S_IRWXU)) == -1){
            perror("Opening file");
            return -1;
        }
    }

    pid_t list_pid[count_processes];
    

    char mmap_file_name[] = "/mmap_file";

    int mmap_file_descriptior;

    if ((mmap_file_descriptior = shm_open(mmap_file_name, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
        perror("open mmap_file_descriptior");
        return -1;
        }

    for(int i = 0; i < count_processes; ++i){
        list_pid[i] = create_processe();
        if(list_pid[i] == 0){

            dup2(mmap_file_descriptior, STDIN_FILENO);
            dup2(list_file_description[i], STDOUT_FILENO);

            execl("../build/child", " ", list_semafore_name[i], list_semafore_name[2], NULL);
            perror("Execl in child");
            return -1;
        }
    }

    char string [max_buffer_size];
    
    int count;

    char* mp;

    while((count = read(0, string, max_buffer_size)) > 0){

        if (ftruncate(mmap_file_descriptior, count) == -1) {
            perror("Could not set size of mfile.txt");
            return 1;
        }

        if ((mp = mmap(NULL, count, PROT_WRITE, MAP_SHARED, mmap_file_descriptior, 0)) == MAP_FAILED) {
            perror("mmap");
            return -1;
        }   
        for(int i = 0; i < count; ++i){
            mp[i] = string[i];
        }

        if(count > 11){ 
            sem_post(list_semafore[1]);
        }
        else{
            sem_post(list_semafore[0]);
        }
        
        sem_wait(list_semafore[2]);
    }

    unlink(mmap_file_name);

    for(int i = 0; i < count_processes; ++i){
        if (kill(list_pid[i], SIGTERM) == -1) {
            perror("Ошибка при отправке сигнала");
        }
        close(list_file_description[0]);
        close(list_file_description[1]);
    }

    for(int i = 0; i < count_semafore; ++i){
        sem_close(list_semafore[i]);
        sem_unlink(list_semafore_name[i]);
    }

    return 0;
}