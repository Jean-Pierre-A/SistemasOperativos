 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/mman.h>
 #include <sys/wait.h>
 #include <semaphore.h>
 #define SH_SIZE 1000

int shared1, shared2;
unsigned int value = 0;
sem_t *semEs1, *semLe1, *semEs2, *semLe2;

void iniciar_recursos()
{

shared2 = shm_open("MemCompartida2", O_CREAT | O_RDWR, 0600);
if(shared2 == -1)
{
       perror("Error shared1 create ");
        exit(EXIT_FAILURE);
}
//fprintf( stdout,"Se creó y trunco shared2 con descriptor %d\n", shared1);
if(-1 == ftruncate(shared2, SH_SIZE))
{
       perror("Error ftruncate1");
        exit(EXIT_FAILURE);
}

shared1 = shm_open("MemCompartida1", O_RDONLY, 0600);
if(shared1 == -1){
        perror("Error shared1 read: ");
        exit(EXIT_FAILURE);
    }
//fprintf(stdout, "Se abrió shared2 con descriptor %d\n", shared2);


}

void apagar_recursos()
{

    if(close(shared2 == -1)){
            perror("Error closeS2 ");
             exit(EXIT_FAILURE);
        }

    if(shm_unlink("MemCompartida2") == -1){
            perror("Error unlinkS2 ");
              exit(EXIT_FAILURE);;
        }
}
void iniciar_control()
{
     mode_t perm = 0666;

    semEs2 = sem_open("semEs2", O_CREAT, perm, value);
    if(semEs2 == SEM_FAILED){
        perror("Error  semEs2 open ");
        exit(EXIT_FAILURE);
    }
    semLe2 = sem_open("semLe2", O_CREAT, perm, value);
    if(semLe2 == SEM_FAILED){
        perror("Error semLe2 open ");
        exit(EXIT_FAILURE);
    }

    semEs1 = sem_open("semEs1", 0);
    if(semEs1 == SEM_FAILED){
        perror("Error semEs1 open: ");
        exit(EXIT_FAILURE);
    }
    semLe1 = sem_open("semLe1", 0);
    if(semLe1 == SEM_FAILED){
        perror("Error semLe1 open: ");
        exit(EXIT_FAILURE);
    }


}

void apagar_control()
{

   if(sem_close(semEs2) == -1){
        perror("Error  semEs2 close: ");
         exit(EXIT_FAILURE);
    }
    if(sem_unlink(semEs2) == -1){
        perror("Error  semEs2 unlink: ");
        exit(EXIT_FAILURE);
    }
    if(sem_close(semLe2) == -1){
        perror("Error semLe2 close: ");
         exit(EXIT_FAILURE);
    }
    if(sem_unlink(semLe2) == -1){
        perror("Error semLe2 unlink: ");
        exit(EXIT_FAILURE);
    }


}

void enviarMsg()
{
char buf[SH_SIZE];
char *dir;
void *map;
int len;

map = mmap(NULL, SH_SIZE, PROT_WRITE, MAP_SHARED, shared2, 0);
while(fgets(buf, sizeof buf, stdin) != NULL)
{
    len = strlen(buf);
    if(buf[len-1] == '\n') buf[len-1] = '\0';

    if(sem_post(semEs2) == -1){
        perror("semEs2 post: ");
         exit(EXIT_FAILURE);
     }

     
      if(map == MAP_FAILED){
          perror("mmapS2: ");
          exit(EXIT_FAILURE);
      }
      dir = (char*)map;
      memcpy(dir, buf, sizeof(buf));   
      if(sem_wait(semLe1) == -1){
          perror("semLe1 wait: ");
          exit(EXIT_FAILURE);
       }
}

if(munmap(dir, SH_SIZE) == -1)
 {
      perror("unmapS2: ");
      exit(EXIT_FAILURE);
}
apagar_control();
apagar_recursos();


}

void recibirMsg()
{
char *dir;
void *map;

map = mmap(NULL, SH_SIZE, PROT_READ, MAP_SHARED, shared1, 0);
 for(;;)
 {

    if(sem_wait(semEs1) == -1){
        perror("semEs1 wait: ");
         exit(EXIT_FAILURE);
    }

    if(map == MAP_FAILED){
        perror("mmapR2: ");
         exit(EXIT_FAILURE);
    }

    dir = (char*)map;
    fprintf(stdout, "Usuario [1]: %s\n",dir);

   

    if(sem_post(semLe2) == -1){
        perror("semLe2 post: ");
         exit(EXIT_FAILURE);
    }
 }
    
 if(munmap(dir, SH_SIZE) == -1)
 {
        perror("unmapR2: ");
         exit(EXIT_FAILURE);
 }

return 0;
}

 void main (void)
 {
    iniciar_recursos();
    iniciar_control();
    
 

   pthread_t id_hiloEnv;
   pthread_t id_hiloRec;

   fprintf(stdout,"Chat Iniciado....\n");
   fprintf(stdout,"--------------Chat Usuario [2]--------------\n");

    pthread_create(&id_hiloEnv, NULL, &enviarMsg, NULL);
    pthread_create(&id_hiloRec, NULL, &recibirMsg, NULL);

    pthread_join(id_hiloEnv,NULL);
    pthread_join(id_hiloRec,NULL);

    return 0;


 }