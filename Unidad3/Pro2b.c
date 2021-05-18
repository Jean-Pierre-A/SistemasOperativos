#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>
#include <errno.h>

#define ARR_SIZE 8192

void*enviar()
{
    mqd_t mqWR;
    char arr1[ARR_SIZE];
     mqWR = mq_open("/WriteReadQueue",O_RDONLY);
        
    if(mqWR == -1)
    {
      exit(1);
     }
     
     mq_receive(mqWR, arr1, ARR_SIZE, NULL);

     printf("User1: %s\n", arr1);
        
     mq_close(mqWR);

}

void *leer()
{
    mqd_t mqRW;
    char arr2[80];
    	fgets(arr2, 80, stdin);
        

		mqRW = mq_open("/ReadWriteQueue",O_WRONLY);
        if(mqRW == -1){
           exit(1);
        }
        mq_send(mqRW, arr2, ARR_SIZE, 0);
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