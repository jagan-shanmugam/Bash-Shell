/*
	Team 2: Exercise sheet2 

	Members:
		1.Jagan Shanmugam
		2.Aastha 


*/

//List of header files and global variables

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define length 2048
#define TOKEN_DELIM " \t\n"

//structure for IO redirection commands 
struct ioreturn
{
	int ioflag, ipos, opos;
	char *infile, *outfile;
};
struct ioreturn commandIO (char *cmd[], int c);

struct background
{
	int bposition, bflag;
	char *bval;	
};

//Function to search for background operator
struct background background_func(char *cmd[], int c);

//Macro for Maximum of two numbers
#define MAX(a,b) (((a)>(b))?(a):(b))

//To access environment variables
extern char **environ;


//Function declarations

//Functions declarations, defined in sum-max file
void generateRandMatrix();
void max_matrix();
void column_matrix();

//Functions declarations, defined in functions file
void matrix();
void listcommand(char *cwd);
void environment(char *cmd1,char *cmd2, int set);
char *setenvironment_init();
int countchars( char* s, char c );
void executepipe(char *cmd[],int numpipes,int c);

//Functions declarations, defined in execute_command file
void execute_command(char *cmd[],char cwd[],int c,char *homedir);

