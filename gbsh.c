
/*
	Team 2: Exercise sheet2 

	Members:
		1.Jagan Shanmugam
		2.Aastha 


*/

//Header files
#include "headerfile.h"


//main code
int main(int argc, char *argv[]) 
{
	//Declarations and Initialization
	char predef_exit[length]="exit",*homedir;
	char command[length], *cmd[length]; 
	char **token,*user, cwd[length],host[128];
	int c=0;
	token=malloc(1024*sizeof(char*));
	struct passwd *pass;
	
	char *backup=setenvironment_init();

	//Infinite loop	prompt
	while(1)
	{

		int pid=0,status;		
		while((pid = waitpid(-1, &status, WNOHANG)) > 0) 
		{
        		printf("[Background proc exited with code %d]\n", WEXITSTATUS(status)); 
		}
	
	//Get user name, host name, current working directory		
	pass=getpwuid(getuid());
	user=pass->pw_name;
	homedir=pass->pw_dir;
	gethostname(host,128);
	getcwd(cwd,sizeof(cwd));

	//Print the prompt everytime with updated information and wait for the input
	printf("%s@%s:%s > ",user,host,cwd);
	if (!fgets(command,length,stdin))
	{		
		printf("NULL");
		break;
	}
	//read, tokenize first command, execute
	else 
	{
		//pre defined commands: exit, pwd, sum-max, cd, ls, environ, setenv, unsetenv
		
		//if empty new line is received, continue the loop		
		if (command[0]=='\n')
			continue;	
		
		//Tokenize the commands entered
		cmd[0]=strtok(command,TOKEN_DELIM);
		
		//To keep a count of tokens for commands everytime and tokenizing			
		c=0;
		while(cmd[c]!=NULL)
		{	
			token[c]=cmd[c];
			c++;
			cmd[c]=strtok(NULL,TOKEN_DELIM);
		}
		token[c]=NULL;
		
		//Predefined command exit
		if (!(strcmp(cmd[0],predef_exit)) && c<2)	
		{	
			break;
		}
		//For other commands calling the execute method in execute_command file
		execute_command(cmd,cwd,c,homedir);
		
	}
			
	}

	//Backed up shell environment variable is restored before exit
	setenv("SHELL",backup,1);	
	printf("Exiting the shell..\n");
	exit(0); // exit normally	
}
