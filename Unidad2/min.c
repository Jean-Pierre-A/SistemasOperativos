 #include <stdio.h>
 #include <stdlib.h>

 int main(int argc, char  *argv[]) 
 {
     int Min = atoi(argv[1]);
     int vec [argc-1];

      for(int i = 1; i< argc; i++)
      {
        vec[i-1] = atoi(argv[i]);
     }

     for(int i = 0; i< argc-1; i++){

        if(Min> vec[i])
         {
             Min = vec[i];
         }
     }

    printf("El Minimo  es : %d \n", Min);
 }