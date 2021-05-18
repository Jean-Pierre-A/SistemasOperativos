 #include <stdio.h>
 #include <stdlib.h>

 int main(int argc, char  *argv[]) 
 {
    float promedio = 0;
     int vec [argc-1];

      for(int i = 1; i< argc; i++)
      {
        vec[i-1] = atoi(argv[i]);
     }

    for(int i = 0; i< argc-1; i++){
          promedio += vec[i];
        
     }

      printf("El Promedio es %f \n", promedio / (argc - 1));
 }