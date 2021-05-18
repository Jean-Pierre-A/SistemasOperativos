 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

 
 int main(int argc, char *argv[]) 
 {
    int nroHijo = 0;
    pid_t pid_hijo, pid_hijo2;
    int status;
    printf("El pid del programa padre es: %d\n",(int)getpid());
  

    for(int i = 1; i <= 3; i++)
    {
       pid_hijo = fork();
       nroHijo ++;

       if(pid_hijo == 0)
       {
           printf("HIJO %d con pid %d\n", nroHijo, getpid);
          break;
       }
       else{
          wait(NULL);
       }
    }

   int Fact = 1;
    for(int i = 1; i <= 10; i++)
    { 
       if(pid_hijo == 0)
       {
       Fact *= i;
       printf(" Hijo %d  = FACT [%d] %d \n",nroHijo, i , Fact);
       }
    }
   
  }
