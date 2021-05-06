#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char const *argv[])
{
    
   
    while(1){

     int tam_Msg_del_Hijo = 100;
    int tam_Msg_del_Padre = 100;

    char para_papa[tam_Msg_del_Hijo];
    char para_hijo[tam_Msg_del_Padre];

    char leer[100];

    memset(leer, 0, 100); 

   int p1[2];//Para Escribir del hijo al padre
   int p2[2];//Para escribir del padre al hijo

    if(pipe(p1) == -1){return 1;}
    if(pipe(p2) == -1){return 1;}

    scanf("%s", leer);
    int pid = fork();

    if(pid == -1){return 2;}

    if(pid == 0){
        close(p2[1]);
        close(p1[0]);

        if(read(p2[0],para_hijo,tam_Msg_del_Padre) == -1){return 3;}
        printf("Mensaje para el hijo: %s\n",para_hijo);
        
        for(int i = 0; para_hijo[i] != '\0'; i++)
        {
            para_papa[i] = toupper(para_hijo[i]);
        }
        if(write(p1[1],para_papa,tam_Msg_del_Hijo) == -1){return 4;}

        close(p2[0]);
        close(p1[1]);
    }else{
        close(p2[0]);
        close(p1[1]);

    
        for(int i = 0; leer[i] != '\0'; i++)
        {
            leer[i] = tolower(leer[i]);
        }
        
        if(write(p2[1],leer,tam_Msg_del_Padre) == -1){return 5;}
        
        if(read(p1[0],para_papa,tam_Msg_del_Hijo) == -1){return 6;}
        printf("Mensaje para el padre: %s\n",para_papa);

        close(p2[1]);
        close(p1[0]);
        wait(NULL);
    }


      memset(leer, 0, 50); 
    }

    return 0;
}
