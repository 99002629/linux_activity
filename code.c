#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MaxMsgs 3
#define BufferSize 3

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int in = 0;
int in1 = 0;
int in2=0;
int in3;
int out = 0;
int out1 = 0;
int out2 =0;
int out3=0;
int buffer[BufferSize];
int buffer1[BufferSize];
int buffer2[BufferSize];
int buffer3[BufferSize];
int msgs;
int msgs1;
int msgs2;
int msgs3;

void *sender(void *send)
{
for(int i = 0; i < MaxMsgs; i++) {
 msgs= rand();
 srand(time(NULL)); 
 msgs1= (rand()%(90-65))+65;
 srand(time(NULL));
 msgs2= (rand()%(122-97))+97;
 srand(time(NULL));
 msgs3= rand()%5;
sem_wait(&empty);
pthread_mutex_lock(&mutex);
buffer[in] = msgs;
buffer1[in1] = msgs1;
buffer2[in2] = msgs2;
buffer3[in2] = msgs3;
printf("sender %d: Insert msgs: %d at %d\n",*((int *)send),buffer[in],in);
printf("sender %d: Insert msgs: %c at %d\n",*((int *)send),buffer1[in1],in1);
printf("sender %d: Insert msgs: %c at %d\n",*((int *)send),buffer2[in2],in2);
printf("sender %d: Insert msgs: %d at %d\n",*((int *)send),buffer3[in3],in3);
in = (in+1)%BufferSize;
in1 = (in1+1)%BufferSize;
in2 = (in2+1)%BufferSize;
in3 = (in3+1)%BufferSize;
pthread_mutex_unlock(&mutex);
sem_post(&full);
}
}

void *receiver(void *recv)
{
for(int i = 0; i < MaxMsgs; i++) {
sem_wait(&full);
pthread_mutex_lock(&mutex);
msgs=buffer[out];
msgs1=buffer1[out1];
msgs2=buffer2[out2];
msgs3=buffer3[out3];
printf("receiver %d: Remove msgs %d :from %d\n",*((int *)recv),msgs, out);
printf("receiver %d: Remove msgs %c :from %d\n",*((int *)recv),msgs1, out1);
printf("receiver %d: Remove msgs %c :from %d\n",*((int *)recv),msgs2, out2);
printf("receiver %d: Remove msgs %d :from %d\n",*((int *)recv),msgs3, out3);
out = (out+1)%BufferSize;
out1 = (out1+1)%BufferSize;
out2 = (out2+1)%BufferSize;
out3 = (out3+1)%BufferSize;
pthread_mutex_unlock(&mutex);
sem_post(&empty);
}
}

int main()
{

pthread_t sen[3],rec[3];
pthread_mutex_init(&mutex, NULL);
sem_init(&empty,0,BufferSize);
sem_init(&full,0,0);

int cnt[2] = {1,2}; 

for(int j = 0; j < 2; j++) {
pthread_create(&sen[j], NULL, (void *)sender, (void *)&cnt[j]);
}
for(int j = 0; j < 2; j++) {
pthread_create(&rec[j], NULL, (void *)receiver, (void *)&cnt[j]);
}

for(int i = 0; i < 2; i++) {
pthread_join(sen[i], NULL);
}
for(int j = 0; j < 2; j++) {
pthread_join(rec[j], NULL);
}

pthread_mutex_destroy(&mutex);
sem_destroy(&empty);
sem_destroy(&full);
return 0;

}


