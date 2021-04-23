#include <sys/wait.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>

 #define BUF_SIZE 40

 int main(int argc, char *argv[])
 {

     int pdf[2], pid_hijo;
     int nbytes, nbytes2;
     char Msg[BUF_SIZE];

   if (pipe(pdf) == -1){ /* Create the pipe */
         perror("pipe");
         exit(EXIT_FAILURE);
     }

  
     if(pid_hijo = fork() > 0)
     {
         write(pdf[1],argv[1],BUF_SIZE);
         close(pdf[1]);
         while(nbytes = read(pdf[0],Msg, BUF_SIZE)>0)
     
         printf("%s\n", Msg);
        if(nbytes != 0)
         {
             exit(2);
         }
         printf("Termino de leer el Padre\n");
         wait(NULL);
     }
     else
     {
        
        write(pdf[1],argv[2],BUF_SIZE);
        close(pdf[1]);
        while( nbytes2 = read(pdf[0],Msg, BUF_SIZE)>0)
      
        printf("%s\n", Msg);
          if(nbytes2 != 0)
         {
             exit(2);
         }
        printf("Termino de leer el Hijo\n");
     }
    
 }