#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
int j=0;
char  Claves [100] [20];
char ArEntrada[20], ArSalida[20], clave[30];
int Valores [100];
FILE *OUT_FILE, *IN_FILE;

strcpy(ArEntrada, argv[1]);
strcpy(ArSalida, argv[3]);

IN_FILE =  fopen(ArEntrada, "r");

  if(IN_FILE == NULL)
  {
        printf("No se Puede Abrir Correctamente \n");
  }
 while(!feof(IN_FILE))
 {
     
      fscanf(IN_FILE, "%s %d", clave, &Valores[j]);
      strcpy(Claves[j], clave);

      //printf("Se Guardo Correctamente %s con %d\n", Claves[j], Valores[j]);
      j++;
     
  }

OUT_FILE =  fopen(ArSalida, "w");

for(int k = j-1; k >= 0; k--)
 {   
     fprintf(OUT_FILE,"%s %d\n", Claves[k],Valores[k]);
 }

 
 fclose(IN_FILE);
 fclose(OUT_FILE);
    
   
}