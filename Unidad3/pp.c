#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define SHM1_SIZE 200
#define SHM2_SIZE 200

void *sendMsg(void*);
void *recvMsg(void*);

sem_t *semW1, *semR1, *semW2, *semR2;

int main(void){

    int shm1, shm2, flg = O_CREAT;
    pthread_t threadSend, threadRecv;

    mode_t permisos = 0666;
    unsigned int value = 0;
    semW1 = sem_open("semW1", flg, permisos, value);
    if(semW1 == SEM_FAILED){
        perror("semW1 open: ");
        exit(1);
    }
    semR1 = sem_open("semR1", flg, permisos, value);
    if(semR1 == SEM_FAILED){
        perror("semR1 open: ");
        exit(1);
    }

    semW2 = sem_open("semW2", 0);
    if(semW2 == SEM_FAILED){
        perror("semW2 open: ");
        exit(1);
    }
    semR2 = sem_open("semR2", 0);
    if(semR2 == SEM_FAILED){
        perror("semR2 open: ");
        exit(1);
    }

    shm1 = shm_open("SharedMemory1", O_CREAT | O_RDWR, 0600);
    if(shm1 == -1){
        perror("shm1 create: ");
        exit(1);
    }
    fprintf(stdout, "Se creó shm1 con descriptor: %d\n", shm1);
    if(-1 == ftruncate(shm1, SHM1_SIZE)){
        perror("ftruncate1: ");
        exit(1);
    }

    shm2 = shm_open("SharedMemory2", O_RDONLY, 0600);
    if(shm2 == -1){
        perror("shm2 read: ");
    }
    fprintf(stdout, "Se abrió shm2 con descriptor: %d\n", shm2);

    pthread_create(&threadSend, NULL, &sendMsg, shm1);
    pthread_create(&threadRecv, NULL, &recvMsg, shm2);

    pthread_join(threadSend,NULL);
    pthread_join(threadRecv,NULL);

    return 0;
}

void *sendMsg(void *shmD){
    int len;
    char buf[SHM1_SIZE];
    char *ptr;
    void *pMap;

    while(fgets(buf, sizeof buf, stdin) != NULL){
        len = strlen(buf);
        if(buf[len-1] == '\n') buf[len-1] = '\0';

        if(sem_post(semW1) == -1){
            perror("semW1 post: ");
            exit(1);
        }

        pMap = mmap(NULL, SHM1_SIZE, PROT_WRITE, MAP_SHARED, shmD, 0);
        if(pMap == MAP_FAILED){
            perror("mmapS1: ");
            exit(1);
        }
        ptr = (char*)pMap;
        memcpy(ptr, buf, sizeof(buf));
        if(munmap(ptr, SHM1_SIZE) == -1){
            perror("unmapS1: ");
            exit(1);
        }
        
        if(sem_wait(semR2) == -1){
            perror("semR2 wait: ");
            exit(1);
        }
    }

    if(sem_close(semW1) == -1){
        perror("semW1 close: ");
        exit(1);
    }
    if(sem_unlink(semW1) == -1){
        perror("semW1 unlink: ");
        exit(1);
    }
    if(sem_close(semR1) == -1){
        perror("semR1 close: ");
        exit(1);
    }
    if(sem_unlink(semR1) == -1){
        perror("semR1 unlink: ");
        exit(1);
    }

    if(close(shmD == -1)){
        perror("closeS1: ");
        exit(1);
    }
    if(shm_unlink("SharedMemory1") == -1){
        perror("unlinkS1: ");
        exit(1);
    }

    return 0;
}
void *recvMsg(void *shmD){
    char *ptr;
    for(;;){

        if(sem_wait(semW2) == -1){
            perror("semW2 wait: ");
            exit(1);
        }

        void *pMap = mmap(NULL, SHM2_SIZE, PROT_READ, MAP_SHARED, shmD, 0);
        if(pMap == MAP_FAILED){
            perror("mmapR1: ");
            exit(1);
        }

        ptr = (char*)pMap;
        fprintf(stdout, "[2]: %s\n",ptr);

        if(munmap(ptr, SHM2_SIZE) == -1){
            perror("unmapR1: ");
            exit(1);
        }

        if(sem_post(semR1) == -1){
            perror("semR1 post: ");
            exit(1);
        }
    }
    return 0;
}