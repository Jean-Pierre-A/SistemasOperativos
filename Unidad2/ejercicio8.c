 //ejercicio 8 by jean pierre 
 #include<stdio.h>
 #include<stdlib.h>
 #include<unistd.h>
 int main(int argc, char  *argv[])
 {
         pid_t pid_hijo;
         char ejecutables[3][10]={"./Pro","./Max","./Min"};
         
         for(int i = 0; i < 3 ; i++)
         {

             pid_hijo = fork();

             if(pid_hijo == 0)
             {
                  execv(ejecutables[i], argv);
             }
             else
             {  
                 wait(NULL);

             }
         }

         printf("Ending-----");

     return 0;
 }