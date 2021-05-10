#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define PERMS 0644

struct my_msgbuf {
   long mtype;
   char mtext[200];
};

void *enviar(void *msqid)
{
struct my_msgbuf buf;
buf.mtype = 1;
int len;



 while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) 
 {
      len = strlen(buf.mtext);
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      if (msgsnd(msqid, &buf, len+1, 0) == -1) 
      perror("msgsnd1");
   }

   strcpy(buf.mtext, "end"); 
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
   perror("msgsnd2");
   
   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   printf("Enviando Mensajes\n");
    

}

void *recibir(void *msqid)
{

struct my_msgbuf buf;
int toend;

   
   for(;;) { /* normally receiving never ends but just to make conclusion 
             /* this program ends wuth string of end */
      if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
         perror("msgrcv");
         exit(1);
      }
      printf("Usuario[1]: \"%s\"\n", buf.mtext);
      toend = strcmp(buf.mtext,"end");
      if (toend == 0)
      break;
   }
   printf("Recibiendo Mensajes\n");
   system("rm msgq.txt");

}


int main ()
{   
    pthread_t id_hilo1;
    pthread_t id_hilo2;
    int msqid, msqid2;
    key_t key1, key2;
   
    system("touch msgq2.txt"); 

    if ((key2 = ftok("msgq2.txt", 'B')) == -1)
   {
      perror("ftok");
      exit(1);
   }

    if ((msqid = msgget(key2, PERMS | IPC_CREAT)) == -1)
   {
      perror("msgget");
      exit(1);
   }

// Primer Cola Creada

     if ((key1= ftok("msgq.txt", 'B')) == -1)
   {
      perror("ftok");
      exit(1);
   }

    if ((msqid2 = msgget(key1, PERMS | IPC_CREAT)) == -1)
   {
      perror("msgget");
      exit(1);
   }

   printf("Las Colas de Mensajes Estan Lista Para Enviar y Recibir Mensajes Mensajes\n");
   printf("------------------------------ CHAT --------------------------------------\n");
    pthread_create(&id_hilo1, NULL,&enviar , msqid);
    pthread_create(&id_hilo2, NULL,&recibir, msqid2);

    pthread_join(id_hilo1, NULL);
    pthread_join(id_hilo2, NULL);
}
