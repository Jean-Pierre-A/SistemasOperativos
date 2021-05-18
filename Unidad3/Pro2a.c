#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>
#include <errno.h>


#define ARR_SIZE 8192

void *enviar()
{

mqd_t mqWR;
char arr1[80];
mqWR = mq_open("/WriteReadQueue",O_WRONLY | O_CREAT,0666,NULL);
        
    if(mqWR == -1){
            exit(1);
        }
        
	 fgets(arr1, 80, stdin);
     mq_send(mqWR,arr1,strlen(arr1) + 1, 0 );
     mq_close(mqWR);

}

void *leer()
{

mqd_t mqRW;
char arr2[ARR_SIZE];
 
mqRW = mq_open("/ReadWriteQueue",O_RDONLY | O_CREAT,0666,NULL);
if(mqRW == -1)
 {
    exit(1);

 }
        
mq_receive(mqRW, arr2, ARR_SIZE, NULL);

printf("User2: %s\n", arr2);
mq_close(mqRW);


}

int main()
{

pthread_t id_hilo1;
pthread_t id_hilo2;



while(1)
{
    pthread_create(&id_hilo1, NULL ,&enviar,NULL);
    pthread_create(&id_hilo2, NULL ,&leer,NULL);
    pthread_join(id_hilo1, NULL);
    pthread_join(id_hilo2, NULL);
}
 

}