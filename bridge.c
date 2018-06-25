#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
void *geekarrive(void *);
void *nongeekarrive(void *);
void *singerarrive(void *);
void crossBridge(void);
typedef sem_t semaphore;
int geekcount,nongeekcount,singercount;
int numboarded;
semaphore mutex;
semaphore boardmutex;
semaphore cross;
semaphore geek;
semaphore nongeek;
semaphore singer;
int main(int argc, char* argv[])
{	
	pthread_t gk,ngk,sg;
	int g,ng,s,i;
	int geekcount1,nongeekcount1,singercount1;
	geekcount1=atoi(argv[1]);
	nongeekcount1=atoi(argv[2]);
	singercount1=atoi(argv[3]);
	sem_init(&mutex, 0, 1);
	sem_init(&cross, 0, 0);
	sem_init(&geek, 0, 0);
	sem_init(&nongeek, 0, 0);
	sem_init(&singer, 0, 0);
	for(i=0;i<geekcount1;i++)
		{	
			pthread_create(&gk,NULL, geekarrive,NULL);
			
		}
	for(i=0;i<nongeekcount1;i++)
		{	
			pthread_create(&ngk,NULL, nongeekarrive,NULL);
			
		}
	for(i=0;i<singercount1;i++)
		{	
			pthread_create(&sg,NULL, singerarrive,NULL);
			
		}
		
	pthread_join(gk, NULL);	
	pthread_join(ngk, NULL);	
	pthread_join(sg, NULL);	
	pthread_exit(NULL);
	sem_destroy(&mutex);
	sem_destroy(&cross);
	sem_destroy(&geek);
	sem_destroy(&nongeek);
  return 0;
exit(0);	 
}
void *geekarrive(void *t1)
{
	sem_wait(&mutex);				//acquire the lock 
	if(geekcount==3)							//this waits for 3 more geek to arrive
	{
		geekcount=geekcount-3;						//decrementing the count of waiting geeks
		sem_post(&mutex);						//releasing the lock
		sem_post(&geek);						//making the 3 geeks ready
		sem_post(&geek);
		sem_post(&geek);
		printf("4 geek has arrived\n");
	}
	else if((geekcount>=1) && (nongeekcount>=2))				//this waits for 1 geek and 2 nongeek to arrive
	{
		geekcount=geekcount-1;
		nongeekcount=nongeekcount-2;						//decrementing the count of waiting nongeek
		sem_post(&mutex);
		sem_post(&geek);					//waking up 1 geek and 2 nongeek
		sem_post(&nongeek);		
		sem_post(&nongeek);
		printf("2 geek and 2 nongeek has arrived\n");
	}
	else									//counting og the no of geek
	{
		geekcount=geekcount+1;						//counting the number of waiting geek
		sem_post(&mutex);					
		sem_wait(&geek);						//lockng the geek till the bridge is full
		sem_wait(&cross);							//waiting for crossbridge till it is full
		return;
	}
	crossBridge();
	sem_post(&cross);
	sem_post(&mutex);
	sleep(2);
}
void *nongeekarrive(void *t1)
{
	sem_wait(&mutex);
	if(nongeekcount==3)										//this waits for 3 more non geek to arrive
	{
		nongeekcount=nongeekcount-3;
		sem_post(&mutex);
		sem_post(&nongeek);
		sem_post(&nongeek);
		sem_post(&nongeek);
		printf("4 nongeek has arrived\n");
	}
	else if((nongeekcount>=1) && (geekcount>=2))						//this waits for 1 noneek and 2 geek to arrive
	{
		nongeekcount=nongeekcount-1;
		geekcount=geekcount-2;
		sem_post(&mutex);
		sem_post(&nongeek);
		sem_post(&geek);
		sem_post(&geek);
		printf("2 nongeek 2 geek has arrived\n");
	}
	else								//this does the counting of no of waiting nongeek			
	{
		nongeekcount=nongeekcount+1;
		sem_post(&mutex);
		sem_wait(&nongeek);
		sem_wait(&cross);
		return;
	}
	crossBridge();
	sem_post(&cross);
	
	sem_post(&mutex);
	sleep(2);
	
}
void *singerarrive(void *t1)
{
	sem_wait(&mutex);
	if( nongeekcount>=3)                                             		//this waits for 3 more nongeeks to arrive
		{
			
			nongeekcount=nongeekcount-3;
			sem_post(&mutex);
			sem_post(&nongeek);
			sem_post(&nongeek);
			sem_post(&nongeek);
			printf("1 singer and 3 nongeek has ariived\n");
		}
	
	else if(geekcount>=1 && nongeekcount>=2)					//this waits for 1 geek and 2 nongeeks to arrive
	{
		
		nongeekcount=nongeekcount-2;
		geekcount=geekcount-1;
		sem_post(&mutex);
		sem_post(&nongeek);
		sem_post(&nongeek);
		sem_post(&geek);
		printf("1 singer 2 nongeek 1 geek has arrived\n");
	}	
	else if(nongeekcount>=1 && geekcount>=2)					//this waits for 1 nongeek and 2 geeks to arrive
	{
		
		geekcount=geekcount-2;
		nongeekcount=nongeekcount-1;
		sem_post(&mutex);
		sem_post(&nongeek);
		sem_post(&geek);
		sem_post(&geek);
		printf("1 singer 2 geek 1 nongeek has arrived\n");
	}
	else if( geekcount>=3)								//this waits for 3 more geeks to arrive
	{		
			geekcount=geekcount-3;
			sem_post(&mutex);
			sem_post(&geek);
			sem_post(&geek);
			sem_post(&geek);
			printf("1 singer and 3 geek has arrived\n");
		

	}
	else
	{
		singercount=singercount+1;
		sem_post(&mutex);
		sem_wait(&singer);
		sem_wait(&cross);
	return;
	}
	crossBridge();
	
	sem_post(&cross);
	sem_post(&mutex);
	sleep(3);
	
}	
void crossBridge()
{
printf("they have reached\n");
sleep(5);
}
