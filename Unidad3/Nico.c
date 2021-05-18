#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    int p1[2];//Para Escribir del hijo al padre
    int p2[2];//Para escribir del padre al hijo

    int tam_Msg_del_Hijo = strlen(argv[2]);
    int tam_Msg_del_Padre = strlen(argv[1]);

    char para_papa[tam_Msg_del_Hijo];
    char para_hijo[tam_Msg_del_Padre];

    if(pipe(p1) == -1){return 1;}
    if(pipe(p2) == -1){return 1;}

    int pid = fork();

    if(pid == -1){return 2;}

    if(pid == 0){
        close(p2[1]);
        close(p1[0]);

        if(read(p2[0],para_hijo,tam_Msg_del_Padre) == -1){return 3;}
        printf("Mensaje para el hijo: %s\n",para_hijo);
        if(write(p1[1],argv[2],tam_Msg_del_Hijo) == -1){return 4;}

        close(p2[0]);
        close(p1[1]);
    }else{
        close(p2[0]);
        close(p1[1]);

        if(write(p2[1],argv[1],tam_Msg_del_Padre) == -1){return 5;}
        if(read(p1[0],para_papa,tam_Msg_del_Hijo) == -1){return 6;}
        printf("Mensaje para el padre: %s\n",para_papa);

        close(p2[1]);
        close(p1[0]);
        wait(NULL);
    }





    return 0;
}
