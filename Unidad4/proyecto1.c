#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


char  TipoR [100];
int guion = 0;
void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void Funcion_tipo(char ruta[100])
{
    struct stat stat_str;
    stat(ruta, &stat_str);
    
    if (S_ISREG(stat_str.st_mode))     /* Determine file type */
	strcpy(TipoR, "regular");
    else if (S_ISDIR(stat_str.st_mode))
	strcpy(TipoR, "directory");
    else 
	strcpy(TipoR, "other");
}

void Listar(char ruta[200])
{
    DIR *stream;
    struct dirent *dep;
    char ruta2 [200], tipo[200];
  

    stream = opendir(ruta);

    while((dep = readdir(stream)) != NULL)
    {

        if(strcmp(dep->d_name, "..")!=0 && strcmp(dep->d_name, "."))
        {
         

            strcpy(ruta2, ruta);
            strcat(ruta2, dep->d_name);

            

            Funcion_tipo(ruta2);
           // printf("ruta %s\n", ruta2);
           // printf("El tipo es %s \n", tipo);
            if(strcmp(TipoR, "directory")==0)
            {   
                if(guion > 0)
                {
                    for(int i = 0; i<=guion; i++)
                    {
                        printf("-> ");
                    }
                }
                printf("Directorio: %s", dep->d_name);

                if(guion > 0)
                {
                    for(int i = 0; i<=guion; i++)
                    {
                        printf(" <-");
                    }
                }
                printf("\n");
                strcat(ruta2, "/");
                guion ++;
                Listar(ruta2);
                
            }
            else
            {
                if(guion > 0)
                {
                    for(int i = 0; i<=guion; i++)
                    {
                        printf("-> ");
                    }
                }
                printf("Archivo %s\n", dep->d_name);

            }
        
        }
    }
        errno = 0;
     if (errno != 0) unix_error("readdir error");
    closedir(stream);

    guion --;


}
int main(int argc, char **argv)
{

    
   char ruta[100];
   printf("Hola\n");
    strcpy(ruta, argv[1]);
    if (argc != 2)
     {
        printf(" %s Recuerda ingresar la ruta\n", argv[0]);
        exit(1);
    }
     printf("Hola\n");

     Funcion_tipo(ruta);

     printf("La ruta es de tipo %s \n", TipoR);
     Listar(ruta);

  

    return 0;
       
 }
