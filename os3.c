#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include <sys/procfs.h>
#define BACKGROUND		40
int mode=0;
struct abc
{
	int idd;
}objj;

int temp;						//this is a flag for checking for foreground and background
struct stu						//this is used to store all the process info
{	char a[100];
	int b;
	int sh;
}obj[100];


struct s						//this is used to store only the background process
{       int st;				
	char aa[100];
	int bb;
	
}ob[100];
int ct1=0;
int cnt=0;
pid_t pid;


void sig_handler(int signum)				//handling signal
{  int fd;
   fd=open("os.txt",O_CREAT | O_RDWR |O_APPEND);
 if(fd<0)
		{ 
			perror("Cannot create output file");
		}
else
		{
		if (signum == SIGCHLD)
			printf("Child Process Terminated !\n");
        	else if(signum==SIGKILL)
         		printf("abnormal\n");
		else if(signum==SIGTSTP)
			dup2(fd,1);
		else return;
		}
  		return;
}

void command(char *line,char *argv[])			//parsing the line
{
	while(*line!='\0')
	{     
		*argv=line;
		while(*line!=' ' && *line!='\t' && *line!='\n')
		{			
			line++;			
		}
		while(*line == ' ' || *line == '\t' || *line == '\n')
		{
			*line = '\0';
			line++;
		}
		if(!strcmp(*argv,"&"))
		{
			*argv='\0';
			mode=BACKGROUND;
		}
		argv++;
	}
	*argv='\0';


}

int pinfo1(char **argv)
{
FILE * fp;
 int count=0; 
char * line; 
size_t len = 0;
 ssize_t read; 
char path1[100]; 
char path2[100];
 strcpy(path1,"/proc/");
 strcpy(path2,"/status");
 char path[100];
int temp;
temp=atoi(argv[1]);
 sprintf(path,"%s%d%s",path1,temp,path2);
 printf(path);
 fp = fopen( path, "r" );
 if (fp == NULL) exit(0); 
while ((read = getline(&line, &len, fp)) != -1) 
{ 
if(count==1 || count ==3 || count ==11)
 printf("%s", line); count++; 
}
	
	}//main

int pinfo(char **argv)
{
char cmd[100];
int k,i;
FILE * fp;
int count=0;
char * line; 
size_t len = 0;
ssize_t read;
k=getpid();
sprintf(cmd,"/proc/%d/status",k);
fp = fopen( cmd, "r" );
if (fp == NULL) 
	exit(0);
while ((read = getline(&line, &len, fp)) != -1)
	{
	 if(count==1 || count ==3 || count ==11) 
		printf("%s", line); 
	 count++;
	 }
printf("Executable path: ./a.out\n");
fclose(fp);
return 0;
}


int  execute(char **argv,char *line)		//this is for forking the child
{ 
pid_t  pid;
int    status;
pid=fork();  
obj[cnt].b=pid;
cnt++;
int fd;
fd = open("os.txt", O_RDWR | O_CREAT);
	if(fd<0)
		{ 
			perror("Cannot create output file");
		}
	else
		{
			if (pid < 0) 
			{   
	      			printf("*** ERROR: forking child process failed\n");
	     			exit(1);
	 		}
       			else if (pid == 0) 
			{      
				if(mode!=BACKGROUND)
		               {    
	      				execvp(*argv, argv);
					_exit(0);
 				}
            			else
             			{
           				dup2(fd,1);
					execvp(*argv, argv);
             	 			exit(1);
					close(fd);
         	 		}
     			}
      			else                                                                             /* for the parent:      */ 
			{       
	      			if (mode!=BACKGROUND && temp==0)                          
           			{
                    			while (wait(&status) != pid)       				/* wait for completion  */
               			;}
	     			else
				{ 
      	                              ob[ct1].st=ct1+1;
		    		      strcpy(ob[ct1].aa,line);	
		                      ob[ct1].bb=pid;
  	                       	      ct1++;
       		   		     signal(SIGCHLD,sig_handler);
		    		     mode=0; 
				}
     			}	
		}
return 0;
}

void jobs()
{
obj[cnt++].b=getpid();		
int i=0;
		for(i=0;i<ct1;i++)
		{               printf("[%d] ",ob[i].st);
				printf("%s",ob[i].aa);							
				printf("[%d]\n",ob[i].bb);
		}	

}

void jobkill(char *id,char *sig,char **argv)					//this is for overkill
{
int id1,sig1;
id1=atoi(id);
sig1=atoi(sig);
int i=0;
for(i=0;i<=ct1;i++)
	{        
		if(ob[i].st==id1)
                          {     
				if(sig1!=9)
                               {
				kill(id1,sig1);
				}
				else
				{
				 ob[i].bb=0;
				 ob[i].st=0;
				 kill(id1,SIGKILL);
				 signal(SIGKILL,sig_handler);
 				}		
			}
    		else
                        {
				printf("wrong job no etered\n");
				break;
			}   
	}
}
int pinfo2(char **argv,char *id)                                                             //this is to show all the process name and Id's
{
obj[cnt++].b=getpid();
		
if(argv[1]==NULL)
{
		int i=0;
		for(i=0;i<cnt;i++)
		{
				printf("command name:");
				puts(obj[i].a);								
				printf("process id= %d\n",obj[i].b);
		}	

}
else
{ 	
int id1;
id1=atoi(id);
int i;
for(i=0;i<ct1;i++)
		{        
			if(ob[i].bb==id1)
			{
				printf("%d",ob[i].st);
	                        printf("command name:");
				puts(obj[i].a);							
				printf("process id= %d\n",obj[i].b);
				
		         }		
		}		
}    	
return 0;
}



int kill2()							//this is for overkill
{
int i;
if(ct1==0)
{
 printf("nothning to kill");
}
for(i=0;i<ct1;i++);
{           	
	kill(ob[i].bb,SIGKILL);
	signal(SIGKILL,sig_handler);
}
return 0;
}


			
int pnt()                                                                    //this is for getting the system and username
{
	int flag=0;
	char path[1000],*user;
	char host[100];
	char *c = getenv("PWD"),d[1000];

	gethostname(host,100);
	user = getenv("USER");
	getcwd(path,1000);
	if(path!=NULL)
	{
		if(flag==0)
		{
			getcwd(d,1000);
			flag = 1;
		}
		printf("<%s@%s:%s>",user,host,path);
	}
	
	return 0;
}


int  main(void)
{    
	fflush(stdin);
	signal(SIGTSTP, sig_handler);
	signal(SIGCHLD, sig_handler);
	fflush(stdin); 
   	char path[1000],*user;
	char host[100];
	char line[1000];
	char *argv[100];
	char *argv1[100];
	int cnt=0;	
	int temp=0;	
	char *c = getenv("PWD"),d[1000];
	int eerr;
	pid_t pid;
	int status;
	signal(SIGINT,SIG_IGN);
	while (1)      
	{	
		argv[0]=NULL;
		pnt();
		gets(line);
		fflush(stdout);
		while(strlen(line)==0)
		{
			pnt();
			gets(line);
		}

		strcpy(obj[cnt++].a,line);
		printf("%s",obj[cnt].a);
		command(line,argv);

     		if (strcmp(argv[0], "quit") == 0) 
             		exit(0);
              
  		else if(strcmp(argv[0],"cd")==0)
 						
			cd(argv); 
  
		else if(strcmp(argv[0],"pinfo2")==0)
 		{ 	char *id;
    			id=argv[1];
           		 pinfo2(argv,id);
		}
		else if(strcmp(argv[0],"fg")==0)
		{
   			char *id;
  			id=argv[1];
   			int id1;
			char lin;
			id1=atoi(id);
			printf("%d",id1);
			int i;
			char *arg;
			int fd;
			fd=open("os.txt",O_RDWR);	
			for(i=0;i<ct1;i++)
				{		        
					if(ob[i].st==id1)
					{ printf("%s",ob[i].aa);
                 			  printf("%d",ob[i].bb);  
        				  printf("guh");
					  dup2(1,fd);
                                          close(fd);
					}
   
				}
		}
	else if(strcmp(argv[0],"jobs")==0)
            	jobs();

	else if(strcmp(argv[0],"kjob")==0)
 	{	char *id,*sig;
		id=argv[1];
		sig=argv[2];
                jobkill(id,sig,argv);
	}		
	else if(strcmp(argv[0],"overkill")==0)
		kill2();
	else if(strcmp(argv[0],"pinfo")==0)
		{
        	if(argv[1]==NULL)
            		  pinfo(argv);
                else
                   {printf("min");    	
			 pinfo1(argv);}
               }
              
	else
     		execute(argv,line);  
  } 
return 0;
}

/*int bg(char *id)
{
  int id1;
id1=atoi(id);
printf("%d",id1);
int i;
char *arg;
int pid_t pid;
for(i=0;i<ct1;i++)
		{        
		if(ob[i].st==id1)
		{ printf("%s",ob[i].aa);
                  printf("%d",ob[i].bb);  
         printf("guh");
        arg=command(ob[i].aa,argv);
   printf("%s",argv);*/
     /* pid=fork();
  if(pid==0)
     {
      execvp(
      }*/
/*}
     return 0;
}*/
int cd(char **argv)
{
int eerr;
int b,fdd;
obj[cnt].b=getpid();
			cnt++;
			if(argv[1]== NULL)
			{
				printf("Entered");	
				chdir("/");
			}
			else
			{
				eerr=chdir(argv[1]);
				if(eerr==-1)
				{
					if((fdd=open(argv[1], O_RDWR))!=-1)
					{printf("It is a file not a directory\n");
						
					}
					else
						printf("Directory does not exist\n");	
				}
			}

return 0;
}


