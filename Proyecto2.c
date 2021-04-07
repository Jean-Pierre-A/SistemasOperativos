#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct personal
 {
     char nombre[20];
     char ocupacion[20];
     int edad;
 } personas;

 personas persona[100];

int j = 0; // J Es la cantidad de lineas
void *inverso(void* file);
void *alfabetico(void* file);
int main(int argc, char *argv[])
{


FILE  *IN_FILE, *IN_FILE2, *IN_FILE3;

 char c, ArEntrada[20], ArSalida[20], ArSalida2[20], AuxNombre[20], AuxOcupacion[20];
 int  AuxEdad = 0;

strcpy(ArEntrada, argv[1]);
strcpy(ArSalida, argv[2]);
strcpy(ArSalida2, argv[3]);

IN_FILE =  fopen(ArEntrada, "r");
if(IN_FILE == NULL)
 {
    printf("No se Puede Abrir Correctamente \n");
 }
 while(!feof(IN_FILE))
 {
   fscanf(IN_FILE, "%s %s %d", AuxNombre, AuxOcupacion, &AuxEdad);
   strcpy(persona[j].nombre, AuxNombre);
   strcpy(persona[j].ocupacion, AuxOcupacion);
   persona[j].edad = AuxEdad;
   

   j++;
     
}

printf("Se Guardo \n");
printf("--------------------------------\n");
for(int i = 0; i < j; i++)
{
    printf("| %s %s %d |\n",persona[i].nombre, persona[i].ocupacion, persona[i].edad);
}
printf("--------------------------------\n");

pthread_t hilo1, hilo2;

pthread_create(&hilo1, NULL,&inverso,&ArSalida);

pthread_join(hilo1, NULL);

pthread_create(&hilo2, NULL,&alfabetico,&ArSalida2);

pthread_join(hilo2, NULL);


printf("------Archivo Inverso-----\n");

IN_FILE2 =  fopen(argv[2], "r");



if(IN_FILE2 == NULL)
  {
        printf("No se Puede Abrir Correctamente \n");
  }
c = fgetc(IN_FILE2);
 while(!feof(IN_FILE2))
    {
        printf("%c", c);
        c = fgetc(IN_FILE2);

    }
 fclose(IN_FILE2);
 IN_FILE3 =  fopen(argv[3], "r");

printf("---------------------------\n");
printf("Archivo Por Orden Alfabetico\n");

if(IN_FILE3 == NULL)
  {
        printf("No se Puede Abrir Correctamente \n");
  }
c = fgetc(IN_FILE3);
while(!feof(IN_FILE3))
    {
        printf("%c", c);
        c = fgetc(IN_FILE3);

    }

  fclose(IN_FILE3);

}



void *inverso(void* file)
{

FILE *OUT_FILE;

OUT_FILE =  fopen(file, "w");
for(int i = j-1; i >= 0; i--)
{
    fprintf(OUT_FILE," %s %s %d \n",persona[i].nombre, persona[i].ocupacion, persona[i].edad);
}
fclose(OUT_FILE);
return NULL;
}

void *alfabetico(void* file)
{
    FILE *OUT_FILE2;
    OUT_FILE2 =  fopen(file, "w");
    char AuxNombre[20], AuxOcupacion[20];
    int AuxEdad = 0;
    for(int m = 0; m < j; m++)
    {
        for(int i = 0; i < j-(m+1); i++)
        {
            if(strcmp(persona[i].ocupacion, persona[i+1].ocupacion)>0)
            {
            strcpy(AuxOcupacion, persona[i].ocupacion);
            strcpy(persona[i].ocupacion,persona[i+1].ocupacion);
            strcpy(persona[i+1].ocupacion,AuxOcupacion);

            strcpy(AuxNombre, persona[i].nombre);
            strcpy(persona[i].nombre,persona[i+1].nombre);
            strcpy(persona[i+1].nombre,AuxNombre);

            AuxEdad =  persona[i].edad;
            persona[i].edad = persona[i+1].edad;
            persona[i+1].edad = AuxEdad;

           
          
            }
          
        }
    }

    for(int i = 0; i < j; i++)
    {
        fprintf(OUT_FILE2," %s %s %d \n",persona[i].nombre, persona[i].ocupacion, persona[i].edad);
    }
    fclose(OUT_FILE2);
    return NULL;
}