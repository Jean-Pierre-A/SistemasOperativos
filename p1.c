#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *OUT_FILE, *IN_FILE;
    char  Claves [100] [20];
    int Valores [100];
    int j = 0;
    char ArEntrada[20], ArSalida[20], clave[30], claveSalida[30];
    int valorSalida = 0;

    strcpy(ArEntrada, argv[1]);
    strcpy(ArSalida, argv[2]);
   // printf("El Archvio de Entrada es %s \n", ArEntrada);

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
    for(int m = 1; m < j; m++)
    {
        for(int i = j-1; i >= m; i--)
        {
            if(Valores[i-1] > Valores[i])
            {
         
            valorSalida =  Valores[i-1];
            Valores[i-1] = Valores[i];
            Valores[i] = valorSalida;

            strcpy(claveSalida, Claves[i-1]);
            strcpy(Claves[i-1], Claves[i]);
            strcpy(Claves[i], claveSalida);
          //  strcpy(claveSalida, NULL);
           
          
            }
          
        }
    }
    OUT_FILE =  fopen(ArSalida, "w");

    for(int k = j-1; k >= 0; k--)
    {   
        //printf("%s %d Pos %d\n", Claves[k],Valores[k], k);
        fprintf(OUT_FILE,"%s %d\n", Claves[k],Valores[k]);
    }


   // printf("El Archvio de Salidad es %s \n", ArSalida);
     
    fclose(IN_FILE);
    fclose(OUT_FILE);
    
}