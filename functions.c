/*
	Team 2: Exercise sheet2 

	Members:
		1.Jagan Shanmugam
		2.Aastha 


*/

#include "headerfile.h"

//To set the shell environment variable at the start of the gbsh
char *setenvironment_init()
{
	char *oldenv=strdup(getenv("SHELL"));
	char cwd[length];
	getcwd(cwd,sizeof(cwd));
	strcat(cwd,"/gbsh");	
	setenv("SHELL",cwd,1);
	printf("Shell enviornment variable initialized to gbsh path, check by entering environ\n");
	return oldenv;
}

//List the directories and files by iteration
void listcommand(char *cwd)
{	
	DIR *dir=NULL;	
	struct dirent *file;
	
	dir=opendir(cwd);
	
	//Check if directory exists
	if (dir == NULL)	
		printf("Invalid directory specified: %s",cwd);
	else
	{	
		for (int c=1; (file=readdir(dir))!=NULL;c++)
		{		
			if(file->d_name[0] !='.') 
				printf("%s   ",file->d_name);
		}
	}	
}

//To view, set, unset the environement variable
void environment(char *cmd1,char *cmd2, int set)
{
	int i=0;

	//printing environment variables	
	if (cmd1==NULL && set==0)
	{
		while (environ[i])
			printf("%s\n",environ[i++]);	
	}

	//setting environment variables
	else if (set==1)
	{
		//check if the variable exists
		if(getenv(cmd1))
		{		
			printf("Variable '%s' already exists\n",cmd1);
		}
			
		//set the variable with the value
		else
		{	
			if(cmd2!=NULL)
			{
				if(setenv(cmd1,cmd2,0))
					perror("gbsh");
			}
			else 
			{
				cmd2="";
				if(setenv(cmd1,cmd2,0))
					perror("gbsh");
			}
			printf("Variable '%s' is set\n",cmd1);		
		}			
	}

	//unsetting environment variables
	else if(cmd2==NULL && set==0)
	{	
		//check if the variable exists
		if(getenv(cmd1))
		{		
			unsetenv(cmd1);
			printf("Variable '%s' is unset\n",cmd1);
		}						
		else
			printf("Variable '%s' doesn't exist\n",cmd1);
	}
}


//Function to search input/output file for IO redirection commands 
struct ioreturn commandIO (char *cmd[], int c)
{
	struct ioreturn var; 
	var.ioflag=0; var.infile=NULL;var.outfile=NULL;
	char *search=NULL;
	for(int i=0;i<c;i++)
	{	
		search=cmd[i];
		
		//Input redirection search 
		if(strcmp(cmd[i],"<")==0)
		{
			var.infile=cmd[i+1];
			var.ioflag=var.ioflag+1;
			var.ipos=i;
			
		}
		else if ((strstr(cmd[i],"<")!=NULL))
		{
			var.infile=strndup(search+1,(strlen(search)-1));
			var.ioflag=var.ioflag+1;
			var.ipos=i;
		}
		
		//Output redirection search
		else if(strcmp(cmd[i],">")==0)
		{
			var.outfile=cmd[i+1];
			var.ioflag=var.ioflag+1;
			var.opos=i;
		}
		else if ((strstr(cmd[i],">")!=NULL))
		{
			var.outfile=strndup(search+1,(strlen(search)-1));
			var.ioflag=var.ioflag+1;
			var.opos=i;
		}
	}
	return var;
}

//Function to search for background operator
struct background background_func(char *cmd[], int c)
{
	struct background var;	
	var.bposition=0; var.bval=NULL; var.bflag=0;
	char *search=NULL;
	for(int i=0;i<c;i++)
	{	
		search=cmd[i];
		
		//background operator search 
		if(strcmp(cmd[i],"&")==0)
		{
			var.bflag=1;
			var.bval=cmd[i+1];			
			var.bposition=i;
			
		}
		else if ((strstr(cmd[i],"&")!=NULL))
		{
			var.bflag=2;			
			var.bval=strndup(search,(strlen(search)-1));
			var.bposition=i;
		}
	} 
	return var;
}

//Function to count the occurences of a character in a string
int countchars( char* s, char c )
{
    return *s == '\0'? 0 : countchars( s + 1, c ) + (*s == c);
}

//Function to handle piped commands
void executepipe(char *cmd[],int numpipes, int c)
{
	const int numcmd=numpipes+1;	
	int temp; int k=0; int j=0; int s=1; int cmdst[10]; int fd[2*numpipes]; int pid,status;
	
	for (int i=0;i<numpipes;i++)
	{
		if (pipe(fd+i*2)<0)
			perror("Pipe creation error");
	}
	cmdst[0]=0;
	
	//splitting the entered string into separate executable commands
	while (cmd[k]!=NULL)
	{
		char *pos;char *search=cmd[k];
		if(!strcmp(cmd[k],"|"))
		{	
			cmd[k]=NULL;
			cmdst[s]=k+1;
			s++;
		}
		else if (strstr(search,"|")!=NULL)
		{	
			pos=strstr(search,"|");				
			int l=pos-search;
			int t=strlen(search)-1;				
			if (l==t) 
			{
				cmd[k]=strndup(search,t);
				for (int a=c+1;a>k+1;a--)
				{	
					cmd[a]=cmd[a-1];
				}				
				cmd[k+1]=NULL;
				cmdst[s]=k+2;
				s++; k++;
				
			}			
			else if (l==0 )
			{
				cmd[k]=strndup(search+1,t+1);
				printf(" %s tokens: %d",cmd[k],c);
				for (int a=c+1;a>=k+1;a--)
				{	
					cmd[a]=cmd[a-1];
				}				
				cmd[k]=NULL;
				cmdst[s]=k+1;
				s++; 				
			}			
			else
			{
				cmd[k]=strndup(search,l);
				
				for (int a=c+2;a>k+2;a--)
				{	
					cmd[a]=cmd[a-2];
				}
				cmd[k+2]=strndup(search+l+1,t+1);
				cmd[k+1]=NULL;
				cmdst[s]=k+2;
				s++;k++;
				c=c+2;				
			}
		}
		k++;
	}
	//duplicating the input and output streams correspondingly and executing the command
	for (int i=0;i<numcmd;++i)
	{
		temp=cmdst[i];
		pid=fork();
		if(pid==0)
		{
			if(i<numpipes)
			{
				if(dup2(fd[j+1],1)<0)
				{	perror("dup2"); exit(0);}
			}
			if (j!=0)
			{
				if(dup2(fd[j-2],0)<0)
				{	perror("dup2"); exit(0);}
			}
			int q;
			for (q=0;q<2*numpipes;q++)
				close(fd[q]);
			if(execvp(cmd[temp],cmd+temp)<0)
				perror("exec");
		}
		else if(pid<0)
			perror("pipe:child creation error");
		j=j+2;
	}
	//Parent: closing the file descriptors and waiting for all child to complete
	if (pid!=0)
	{
		for (int i=0;i<2*numpipes;i++)
		close(fd[i]);
	
		//waiting for all child to finish
		for (int i=0;i<numpipes+1;i++)
			wait(&status);
	}
}

