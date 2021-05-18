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

 #define SH_SIZE 4

 int shared_fd = -1;
 int32_t* counter = NULL;
 sem_t* semaphore = NULL;

 void init_control_mechanism() {
     semaphore = sem_open("sem0", O_CREAT, 0600, 1);
     if (semaphore == SEM_FAILED) {
         perror("Opening the semaphore failed: ");
         exit(EXIT_FAILURE);
     }
 }

 void shutdown_control_mechanism() {
     if (sem_close(semaphore) < 0) {
         perror("Closing the semaphore failed: ");
         exit(EXIT_FAILURE);
     }

     if (sem_unlink("sem0") < 0) {
         perror("Unlinking failed: ");
         exit(EXIT_FAILURE);
     }
 }

 void init_shared_resource() {
     shared_fd = shm_open("shm0", O_CREAT | O_RDWR, 0600);
     if (shared_fd < 0) {
         perror("Failed to create shared memory: ");
         exit(EXIT_FAILURE);
     }
     fprintf(stdout, "Shared memory is created with fd: %d\n", shared_fd);
 }

 void shutdown_shared_resource() {
     if (shm_unlink("/shm0") < 0) {
         perror("Unlinking shared memory failed: ");
         exit(EXIT_FAILURE);
     }
 }

 void inc_counter() {
     usleep(1);
     sem_wait(semaphore);
     int32_t temp = *counter;
     usleep(1);
     temp++;
     usleep(1);
     *counter = temp;
     sem_post(semaphore);
     usleep(1);
 }


 int main(int argc, char** argv) {
     init_shared_resource();
     init_control_mechanism();

     if (ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0) {
         perror("Truncation failed: ");
         exit(EXIT_FAILURE);
     }
     fprintf(stdout, "The memory region is truncated.\n");
     void* map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);

     if (map == MAP_FAILED) {
         perror("Mapping failed: ");
         exit(EXIT_FAILURE);
     }

     counter = (int32_t*)map;
     *counter = 0;

     pid_t pid = fork();
     if (pid) { // The parent process
         inc_counter();
         fprintf(stdout, "The parent process sees the counter as %d.\n", *counter);
         int status = -1;
         wait(&status);
         fprintf(stdout, "The child process finished with status %d.\n", status);
     } else { // The child process
         inc_counter();
         fprintf(stdout, "The child process sees the counter as %d.\n", *counter);
         if (sem_close(semaphore) < 0) {
             perror("Closing the semaphore failed: ");
         }
     }


     if (munmap(counter, SH_SIZE) < 0) {
         perror("Unmapping failed: ");
         exit(EXIT_FAILURE);
     }

     if (close(shared_fd) < 0) {
         perror("Closing shared memory fd failed: ");
         exit(EXIT_FAILURE);
     }

     if (pid) {
         shutdown_shared_resource();
         shutdown_control_mechanism();
     }

     exit(EXIT_SUCCESS);
 }