
#include "headerfile.h"

//Function to execute the pre defined commands: pwd, sum-max, cd, ls, environ, setenv, unsetenv, to create a child process for all others
void execute_command(char *cmd[],char cwd[],int c,char *homedir)
{
	char predef_pwd[length]="pwd",predef_sum_max[length]="sum-max", predef_changedir[length]="cd",predef_list[length]="ls",predef_environ[length]="environ",predef_setenv[length]="setenv",predef_unsetenv[length]="unsetenv"; 
	
	struct ioreturn io;	
	//Searching for I/O redirections in the command entered and store it in a struct
	io=commandIO(cmd,c);
	//searching for background &	
	struct background backgr;
	backgr=background_func(cmd,c);	
	
	//to count the pipes
	int pipeflag=0;
	for (int a=0;a<c;a++)
	{	if(strcmp(cmd[a],"|")==0)
			pipeflag+=1;
		else if (strstr(cmd[a],"|")!=NULL)
			pipeflag+=1;
	}
	//commands NOT supported by background: sum-max, cd, setenv, unsetenv
	if (backgr.bflag==0 && io.ioflag==0 && pipeflag==0)
	{
		//Display the present working directory
		if(!(strcmp(cmd[0],predef_pwd)) && c<2)
		{
			printf("%s\n",cwd);
		}

		//Sum-max command given in exercise 1
		else if(!(strcmp(cmd[0],predef_sum_max)) && c<2)	
		{
			printf("Sum-max command: \n");
			matrix();
		}

		//ls command to display the contents of the directory
		else if(!(strcmp(cmd[0],predef_list)) && io.ioflag==0 && c<3)	
		{
			//printf("List: \n");
			if (cmd[1]==NULL)
				listcommand(cwd);
			else
				listcommand(cmd[1]); 		
			printf("\n");
		}

		//cd command to change the current directory
		else if(!(strcmp(cmd[0],predef_changedir)) && c<3)
		{
			//when destination is unspecified, move to the home directory				
			if(cmd[1]==NULL)
			{						
				chdir(homedir);
			}
			//Otherwise append the destination and change to directory
			else
			{
				char *change=strcat(cwd,"/");
				change=strcat(change,cmd[1]);
				if(chdir(change))
					perror("gbsh");
			}

		}
	
		//To view the environment variables	
		else if(!(strcmp(cmd[0],predef_environ)) && c<2 && (io.ioflag==0))	
		{
			//printf(" %s %s %s\n",cmd,cmd1,cmd2);
			environment(cmd[1],cmd[2],0);
			printf("\n");
		}
		
		//For setting an environment variable
		else if(!(strcmp(cmd[0],predef_setenv)) && (c>1 && c<4) )
		{
			environment(cmd[1],cmd[2],1);
		}
	
		//For unsetting an environment variable		
		else if(!(strcmp(cmd[0],predef_unsetenv)) && (c>1 && c<3))
		{
			environment(cmd[1],cmd[2],0);
		}
	}
		
	//For other inputs, invoking it as a prgoram in a child process 		
	else
	{		
				
		//Program invocation 1.Child process 2.Execute			
		pid_t pid=fork();
		int status,in,out;
		
		//Using env call, if 'environ' command is used with output redirection
		if(!(strcmp(cmd[0],predef_environ)) && ((io.ioflag>0) || (backgr.bflag>0)))	
		{
			cmd[0]="env";
		}
		
		//Child process	
		if (pid==0)
		{
			if (backgr.bflag>0)
			{	
				//setpgid(pid,0);		
				if (backgr.bflag==1)
					cmd[backgr.bposition]=NULL;
				else if (backgr.bflag==2)
					cmd[backgr.bposition]=backgr.bval;
			}

			//Opening the input file and duplicating the input stream for exec
			if((io.ioflag>0) && io.infile !=NULL)
			{	
				if (backgr.bflag==2)
					io.infile=strndup(io.infile,(strlen(io.infile)-1));				
				if((in=open(io.infile,O_RDONLY))<0)
				{
					perror("Couldn't open input file");
					exit(0);
				}
				dup2(in,0);
				close(in);
				cmd[io.ipos]=NULL;
			}
			//Creating the output file and duplicating the input stream for exec
			if((io.ioflag>0) && io.outfile !=NULL)
			{	
				if (backgr.bflag==2)
					io.outfile=strndup(io.outfile,(strlen(io.outfile)-1));
				if((out=creat(io.outfile,0644))<0)
				{
					perror("Couldn't open output file");
				}
				dup2(out,STDOUT_FILENO);
				close(out);
				cmd[io.opos]=NULL;
			}
			
			//handling pipe
			if(pipeflag>0)
			{
				executepipe(cmd,pipeflag,c);				
			}
			//Executing the command 		
			else if(execvp(cmd[0],cmd)==-1)
			{	
				perror("execvp");
				printf("Unrecognized command:\n");
				for(int z=0;z<c;z++)
					printf("%s ",cmd[z]);
				printf("\n");
			}	
			//exiting the child process
			

			exit(0);			
		}
			
		//if creation of child process is failed 
		else if(pid<0)
			perror("Child process creation error");
		
		//Parent process: waiting for child process to complete to avoid zombie process
		else if(pid!=0)
		{
			//if it's a background process return to the prompt
			if (backgr.bflag>0)
			{

			}
			//if it's not background, wait for the child to complete
			else if (backgr.bflag==0)
			{
				do
				{ 	//Waiting for child process to avoid going into defunct state
					waitpid(pid,&status, WUNTRACED);
				}while(!WIFEXITED(status) && !WIFSIGNALED(status));
			}
			
		}
	}
	
}
