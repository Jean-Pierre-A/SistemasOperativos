 #include <stdio.h>
 #include <stdlib.h>

 int main(int argc, char  *argv[]) {

     float promedio = 0;
     int Min = 0;
     int Max = 0;
     Min = atoi(argv[1]);
     for(int i = 1; i< argc; i++){
         promedio += atoi(argv[i]);
         if(Min>atoi(argv[i]))
         {
             Min = atoi(argv[i]);
         }
         if(Max <  atoi(argv[i]))
         {
              Max = atoi(argv[i]);
         }
     }

     printf("El Promedio es %f \n", promedio / (argc - 1));
     printf("El Maximo es : %d \n", Max);
     printf("El Minimo es : %d \n", Min);
     exit(EXIT_SUCCESS); // exit(EXIT_FAILURE) en caso de error
 }