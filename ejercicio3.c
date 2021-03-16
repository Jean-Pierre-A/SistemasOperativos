 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

 int main(int argc, char *argv[]) {
     pid_t pid_hijo;

     printf("Este proceso va a crear otro proceso\n\n");
     printf("El pid del programa padre es: %d\n",(int)getpid());
     pid_hijo = fork();
     switch(pid_hijo) {
         case -1: /* Error */
             printf("Error al crear el proceso");
             return -1;
         case 0: /* Código ejecutado por el hijo */
             printf("PROCESO HIJO:\n");
             printf("Soy el hijo, mi PID es %d\n",(int)getpid());
         break;
         default: /* Código ejecutado por el padre */
             printf("PROCESO PADRE: Proceso hijo con PID %d creado\n",(int)pid_hijo);
     }
     /* Esta línea será ejecutada por ambos procesos, pero en
        en diferente contexto (el de cada proceso) */
     printf("Soy el proceso %d terminando \n",(int)getpid());
     exit(EXIT_SUCCESS);
 }