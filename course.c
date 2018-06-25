#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<wait.h>
#include <fcntl.h>
#include <sys/procfs.h>
void *Student(void *);
void prin(void );
typedef sem_t semaphore;
semaphore comm;
semaphore human;
semaphore art;
semaphore manag;
semaphore mutex;
int stdid=1;
int s,c;
int commcount,managcount,artcount,humancount;
int no_of_stud[4];
int copy_stud[4];
int stud_cat[4];
 int cont=1;
int sum;

int main(int argc, char* argv[])
{
	pthread_t stu;
	int i,m,x,y,z,ct;
	s=atoi(argv[1]);
	c=atoi(argv[2]);
	ct=1;
	m=c%4;					//no of courses jo repeat hai.
	x=c/4;	
	y=x+1;
	z=x;
	if(m==0)
	{    
		for(i=0;i<4;i++)
		{
			no_of_stud[i]=x*60;
			copy_stud[i]=no_of_stud[i];	
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{		//ct=1;
			if(ct<=x)
			{
			no_of_stud[i]=y*60;
			copy_stud[i]=no_of_stud[i];
			ct++;
			}
			else
			{
			no_of_stud[i]=z*60;
			copy_stud[i]=no_of_stud[i];				
			}
		}
	}
	for(i=0;i<4;i++)
	{
	sum=sum+no_of_stud[i];
	}
	sem_init(&mutex, 0, 1);				//creation of mutex semaphore
	sem_init(&comm, 0, 1);				//creation of commerce semaphore
	sem_init(&human, 0, 1);				//creation of humanities semaphore
	sem_init(&art, 0, 1);				//creation of arts semaphore
	sem_init(&manag, 0, 1);				//creation of management semaphore
	for(i=0;i<s;i++)
	{	
		pthread_create(&stu, NULL, Student, NULL);
	}
	pthread_join(stu, NULL);
	//sem_destroy(&s);
	
  	pthread_exit(NULL); 
return 0;
}
void *Student(void *t1)
{	sem_wait(&mutex);	
	if(cont>60)
  	{	
	int k,mcom,phd,bcom,arts;
	k=s-cont+1;
	
	cont++;
	
	printf("no of student which has not been allocated any courses are = %d  \n",k);
  	exit(0);
	
	}
	else
	{
	
	int count=0;
	sem_wait(&comm);						//locks the commerce field
	no_of_stud[count]=no_of_stud[count]-1;	

	sem_post(&comm);
	sem_wait(&human);						////locks the humanitites field
	no_of_stud[count+1]=no_of_stud[count+1]-1;	

	sem_post(&human);
	sem_wait(&art);							////locks the arts field	
	no_of_stud[count+2]=no_of_stud[count+2]-1;

	sem_post(&art);
	sem_wait(&manag);						////locks the management field
	no_of_stud[count+3]=no_of_stud[count+3]-1;

	sem_post(&manag);

	cont++;					//this counts and checks if the number of student has not exceeded the course seat no
	
	}
prin();
sem_post(&mutex);

}
void prin()
{
	int i;
	FILE *fd;
	fd=fopen("allocation.txt","a+");
	fprintf(fd,"student %d has been allocated  all courses \n",stdid);
	stdid=stdid+2;
	for(i=0;i<4;i++)
	{	
		
		fprintf(fd,"number of seats left in %d spectrum %d \n",i,no_of_stud[i]);
		//fprintf(fd,"total no of ppl in %d spectrum %d \n",i,copy_stud[i]);
		
		
	}
	fclose(fd);
}	

