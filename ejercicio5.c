 #include <stdio.h>
 #include <stdlib.h>

 int main(int argc, char  *argv[]) {

     float promedio = 0;
     int Min = 0;
     int Max = 0;
     int vec [argc-1];
     Min = atoi(argv[1]);
     for(int i = 1; i< argc; i++){
        vec[i-1] = atoi(argv[i]);
     }

     for(int i = 0; i< argc-1; i++){

        // printf("Vec[%d] es %d \n", i ,vec[i]);
         promedio += vec[i];
         if(Min> vec[i])
         {
             Min = vec[i];
         }
         if(Max <  vec[i])
         {
              Max = vec[i];
         }
     }

     printf("El Promedio es %f \n", promedio / (argc - 1));
     printf("El Maximo es : %d \n", Max);
     printf("El Minimo es : %d \n", Min);
     exit(EXIT_SUCCESS); // exit(EXIT_FAILURE) en caso de error
 }