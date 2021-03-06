//Fahadul Islam
//2018-2-60-102
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<stdlib.h>
#include<unistd.h>
using namespace std;



//declaration1 - for customer and chef.
sem_t empty;
sem_t full;
pthread_mutex_t lock1;

queue<int> q1;
int cnt = 0;

//declaration2 - for chef and masterchef.. 
sem_t empty1;
sem_t full1;
pthread_mutex_t lock2;

queue<int> q2;
int cnt2 = 0;

//declaration3 - for masterchef and waiter...
sem_t empty2;
sem_t full2;
pthread_mutex_t lock3;

queue<int> q3;
int cnt3 = 0;
 

//initialization1 - for customer and chef.
void init_semaphore()
{
	sem_init(&empty,0,5);
	sem_init(&full,0,0);
	pthread_mutex_init(&lock1,0);
}

//initialization2 - for chef and masterchef..
void init_semaphore2()
{
	sem_init(&empty1,0,3);
	sem_init(&full1,0,0);
	pthread_mutex_init(&lock2,0);
} 

//initialization3 - for masterchef and waiter...
void init_semaphore3()
{
	sem_init(&empty2,0,3);
	sem_init(&full2,0,0);
	pthread_mutex_init(&lock3,0);
}



//customer as a producer part1
void * customer(void * arg)
{
    int* customer_id = (int*)arg;
	printf("I am customer %d\n",(int*)arg);
	int i;
	

		sem_wait(&empty);

		pthread_mutex_lock(&lock1);
		sleep(1);
		q1.push(cnt);
		printf("customer %d has placed his order %d\n", customer_id, cnt);
		cnt++;
		pthread_mutex_unlock(&lock1);

		sem_post(&full);

}


//chef as a consumer part 1
void * chef(void * arg)
{
    int* chef_id = (int*)arg;
    printf("I am chef\n");
	int i;
	while(true)
	{
        sem_wait(&full);

		pthread_mutex_lock(&lock1);
		sleep(1);
		int item = q1.front();
		q1.pop();
		printf("chef has prepared order %d\n",chef_id, item);
		item++;
		pthread_mutex_unlock(&lock1);

        sem_post(&empty);
        
        sem_wait(&empty1);

		pthread_mutex_lock(&lock2);
		sleep(1);
		q2.push(item);
		printf("chef %d has sent order %d\n to masterchef", chef_id, cnt2);
		cnt2++;
		pthread_mutex_unlock(&lock2);

		sem_post(&full1);
	}
}
//chef as a producer part 2



//masterchef as a consumer part 2
void * masterchef(void * arg)
{

    printf("I am masterchef\n");
	int i;
	while(true)
	{
        sem_wait(&full1);

		pthread_mutex_lock(&lock2);
		sleep(1);
		int item = q2.front();
		q2.pop();
		printf("masterchef has checked order %d\n", item);
		pthread_mutex_unlock(&lock2);

        sem_post(&empty1);
	}
}

//masterchef as a producer - part3
void * masterchef(void * arg)
{
    int* chef_id = (int*)arg;
	printf("I am masterchef %d\n",(int*)arg);
	int i;
	while(true)
	{
		sem_wait(&empty2);

		pthread_mutex_lock(&lock3);
		sleep(1);
		q3.push(cnt3);
		printf("masterchef %d has called waiter to collect order %d\n", masterchef_id, cnt3);
		cnt3++;
		pthread_mutex_unlock(&lock3);

		sem_post(&full);
	}
}

//waiter as a consumer - part3
void * waiter(void * arg)
{

    printf("I am waiter\n");
	int i;
	while(true)
	{
        sem_wait(&full2);

		pthread_mutex_lock(&lock3);
		sleep(1);
		int item = q3.front();
		q3.pop();
		printf("\n waiter has delivered order %d\n", item);
		pthread_mutex_unlock(&lock3);

        sem_post(&empty2);
	}
}




int main(void)
{   

    //for customer and chef.........
	pthread_t thread1[10];
	pthread_t thread2[3];
    int x[10] = {1, 2, 3, 4, 5,6,7,8,9,10};
	int y[3]={1,2,3};

	init_semaphore();

	for (int i = 0; i < 10; i++){
        pthread_create(&thread1[i], NULL, customer, (void*) *(x+i));
	}

	for(int j=0; j< 3 ; j++){
	   pthread_create(&thread2[j], NULL, chef,(void*) *(y+j) );
	}
	//for chef and masterchef.............
	pthread_t thread3[3];
	pthread_t thread4;
	
    int z[3] = {1, 2, 3};

	init_semaphore2();

	for (int i = 0; i < 3; i++){
        pthread_create(&thread3[i], NULL, chef, (void*) *(z+i));
	}

	pthread_create(&thread4, NULL, masterchef, NULL);
	
	//for masterchef and waiter
	pthread_t thread5[3];
	pthread_t thread6;
	
    int k[3] = {1, 2, 3};

	init_semaphore3();

	for (int i = 0; i < 3; i++){
        pthread_create(&thread5[i], NULL, masterchef, (void*) *(k+i));
	}

	pthread_create(&thread6, NULL,waiter, NULL);

	while(1);
	return 0;
}

