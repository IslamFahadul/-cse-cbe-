#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

sem_t empty;
sem_t full;
queue<int> q;
pthread_mutex_t lock;
int cnt = 0;

void init_semaphore()
{
	sem_init(&empty,0,3);
	sem_init(&full,0,0);
	pthread_mutex_init(&lock,0);
}

void * carboFunc(void * arg)
{
	printf("%s\n",(char*)arg);
	int i;

    while(true){
		sem_wait(&empty);

		pthread_mutex_lock(&lock);
		sleep(1);
		q.push(cnt);
		cnt++;
		printf("Carbon Created %d\n", cnt);
		pthread_mutex_unlock(&lock);

		sem_post(&full);

	}
}

void * oxyFunc(void * arg)
{
	printf("%s\n",(char*)arg);
	int i;
	while(true)
	{

		sem_wait(&full);

		pthread_mutex_lock(&lock);
		sleep(1);
		int item = q.front();
		q.pop();
		printf("Oxyzane Created %d\n",item);
		pthread_mutex_unlock(&lock);

		sem_post(&empty);

	}
}





int main(void)
{
	pthread_t thread1;
	pthread_t thread2;

	init_semaphore();

	char * message1 = "Carbon";
	char * message2 = "Oxyzane";

	pthread_create(&thread1,NULL,carboFunc,(void*)message1 );
	pthread_create(&thread2,NULL,oxyFunc,(void*)message2 );


	while(1);
	return 0;
}
