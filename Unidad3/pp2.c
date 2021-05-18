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
    semW2 = sem_open("semW2", flg, permisos, value);
    if(semW2 == SEM_FAILED){
        perror("semW2 open: ");
        exit(1);
    }
    semR2 = sem_open("semR2", flg, permisos, value);
    if(semR2 == SEM_FAILED){
        perror("semR2 open: ");
        exit(1);
    }

    semW1 = sem_open("semW1", 0);
    if(semW1 == SEM_FAILED){
        perror("semW1 open: ");
        exit(1);
    }
    semR1 = sem_open("semR1", 0);
    if(semR1 == SEM_FAILED){
        perror("semR1 open: ");
        exit(1);
    }

    shm2 = shm_open("SharedMemory2", O_CREAT | O_RDWR, 0600);
    if(shm2 == -1){
        perror("shm2 create: ");
        exit(1);
    }
    fprintf(stdout, "Se creó shm2 con descriptor: %d\n", shm2);
    if(-1 == ftruncate(shm2, SHM2_SIZE)){
        perror("ftruncate2: ");
        exit(1);
    }

    shm1 = shm_open("SharedMemory1", O_RDONLY, 0600);
    if(shm1 == -1){
        perror("shm1 read: ");
    }
    fprintf(stdout, "Se abrió shm1 con descriptor: %d\n", shm1);

    pthread_create(&threadSend, NULL, &sendMsg, shm2);
    pthread_create(&threadRecv, NULL, &recvMsg, shm1);

    pthread_join(threadSend,NULL);
    pthread_join(threadRecv,NULL);

    return 0;
}

void *sendMsg(void *shmD){
    int len;
    char buf[SHM2_SIZE];
    char *ptr;
    void *pMap;

    while(fgets(buf, sizeof buf, stdin) != NULL){
        len = strlen(buf);
        if(buf[len-1] == '\n') buf[len-1] = '\0';

        if(sem_post(semW2) == -1){
            perror("semW2 post: ");
            exit(1);
        }

        pMap = mmap(NULL, SHM2_SIZE, PROT_WRITE, MAP_SHARED, shmD, 0);
        if(pMap == MAP_FAILED){
            perror("mmapS2: ");
            exit(1);
        }
        ptr = (char*)pMap;
        memcpy(ptr, buf, sizeof(buf));
        if(munmap(ptr, SHM2_SIZE) == -1){
            perror("unmapS2: ");
            exit(1);
        }
        if(sem_wait(semR1) == -1){
            perror("semR1 wait: ");
            exit(1);
        }
    }

    if(sem_close(semW2) == -1){
        perror("semW2 close: ");
        exit(1);
    }
    if(sem_unlink(semW2) == -1){
        perror("semW2 unlink: ");
        exit(1);
    }
    if(sem_close(semR2) == -1){
        perror("semR2 close: ");
        exit(1);
    }
    if(sem_unlink(semR2) == -1){
        perror("semR2 unlink: ");
        exit(1);
    }

    if(close(shmD == -1)){
        perror("closeS2: ");
        exit(1);
    }
    if(shm_unlink("SharedMemory2") == -1){
        perror("unlinkS2: ");
        exit(1);
    }

    return 0;
}
void *recvMsg(void *shmD){
    char *ptr;
    for(;;){
        if(sem_wait(semW1) == -1){
            perror("semW1 wait: ");
            exit(1);
        }

        void *pMap = mmap(NULL, SHM1_SIZE, PROT_READ, MAP_SHARED, shmD, 0);
        if(pMap == MAP_FAILED){
            perror("mmapR2: ");
            exit(1);
        }

        ptr = (char*)pMap;
        fprintf(stdout, "[1]: %s\n",ptr);

        if(munmap(ptr, SHM1_SIZE) == -1){
            perror("unmapR2: ");
            exit(1);
        }
        if(sem_post(semR2) == -1){
            perror("semR2 post: ");
            exit(1);
        }
    }
    return 0;
}