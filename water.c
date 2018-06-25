#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<math.h>

int total_energy;
sem_t mutex_reactions; // Mutex to keep track of number of reactions !
int fr=0,r;
int count=1;
sem_t mutex_count;
int hydrogen,oxygen;
void increment()
{
	if(fr==r)
	fr=0;
	else
	fr++;
}
void * reaction(int i)
{
	sem_wait(&mutex_reactions);	
	sem_wait(&mutex_count);		
	printf("Hydrogen Left : %d\n",hydrogen);
	printf("Oxgen Left : %d\n",oxygen);
	printf("%d H2O formed till now ",count);		
	printf("at Site %d\n",fr+1);
	total_energy++;
	printf("Total energy : %d \n",total_energy);
	count++;
	hydrogen--;
	hydrogen--;
	oxygen--;
	if(fr!=r)
		increment();
	increment();	
	sem_post(&mutex_count);
	sleep(3);	
	sem_post(&mutex_reactions);
}


int main(int argc,char **argv)
{
	if(argc<5)
	{
		printf("Please run as follows : ./a.out <No of H atoms> <No of O atoms> <No of sites> <Threshold  Energy(in Units)>\n");
		exit(EXIT_FAILURE);
	}
	hydrogen=atoi(argv[1]);
	oxygen=atoi(argv[2]);	
	int threshold,no_of_sites,i;
	threshold=atoi(argv[4]);
	no_of_sites=atoi(argv[3]);
	int ceil_sites=no_of_sites/2;
	if(no_of_sites%2!=0)
	ceil_sites++;
	int mutex_value=(threshold > ceil_sites?  ceil_sites : threshold);
	sem_init(&mutex_reactions,0,mutex_value);	
	sem_init(&mutex_count,0,1);	
	r=no_of_sites-1;	
	if(hydrogen%2!=0)
		hydrogen--;
	int usable_h=hydrogen,usable_o=oxygen;
	if(hydrogen>2*oxygen)
		usable_h=oxygen*2;
	else if(hydrogen<2*oxygen)
		usable_o=hydrogen/2;
	pthread_t molecules[usable_o];
	for(i=0;i<usable_o;i++)
	{	
		pthread_create(&molecules[i],NULL,reaction,(void*)i);
	}
	for(i=0;i<usable_o;i++)
	{		
		
		pthread_join(molecules[i],NULL);
		
	}
	
	printf("Hydrogen Left : %d\n",hydrogen);
	printf("Oxgen Left : %d\n",oxygen);
	return 0;
	
}


