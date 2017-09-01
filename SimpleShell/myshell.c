#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
void  convert(char *str, char **argv);
void  execute(char **argv);
void executepipe(char **argv1, char **argv2);
int main (void)
{
	char  *str;             
	char  *argv[64];
	char *pipe[64];
	int i=0;
	bool con=true;
	while (con) 
	{                   		 
		printf("Hello_Danish >> ");     
		str = readline("\n>>> ");
		if(strlen(str)!=0)
	    	{
	    		add_history(str);
		   	}
		   	else
		   		continue;
 		printf("\n");	
		int x = checkpipe(str,argv,pipe);
		if(x==0)
		convert(str, argv);       
		if (strcmp(argv[0], "exit") == 0)  
			exit(0); 
		if(strcmp(argv[0], "cd") == 0)
		{
			chdir(argv[1]);
			continue;	
		}
		if(x==0)		
		execute(argv);
		if(x==1)
			executepipe(argv,pipe);
	}
return 0;
}
void  execute(char **argv)
{
	pid_t  pid;
	int    status;

	if (( pid = fork()) < 0) 
	{     	
		printf("Error\n");
		exit(1);
	}
	else if (pid == 0) 
	{          
		if (execvp(*argv, argv) < 0) 
		{     
			printf(": Command not found\n");
			exit(1);
		}
	}
	else 
	{               
		while (wait(&status) != pid);    		
	}
}
int  checkpipe(char *str, char **argv,char **pipe)
{
	char *strpiped[2];
	int i,pipenum=0,pcount=0;
	for(i = 0; i<2; i++)
	{
        	strpiped[i] = strsep(&str, "|");
        	if(strpiped[i] == NULL)
        		break;
    	}
    	if(strpiped[1] == NULL)
    		pipenum=0;
    	else
    	{
    		pipenum= 1;
    	}
	if(pipenum==1)
	{	convert(strpiped[0], argv);
		convert(strpiped[1], pipe);
	}
    	return pipenum;
}
void executepipe(char **argv1, char **argv2)
{
	int pipefd[2];                     
	pid_t p1, p2;

	pipe(pipefd);
	p1 = fork();
	if( p1 == 0 )
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		if(execvp(argv1[0], argv1)<0)
        	{
        		printf("\nCommand not found");
        	}
	}
	else
	{
		p2 = fork();
		if (p2 == 0)
                {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if(execvp(argv2[0], argv2)<0)
			{
        			printf("\nCommand not found");
        		}
                }
		else
		{			
			wait(NULL);
		}
	}
}
void convert(char *str,char **argv)
	{	while (*str != '\0') 
		{       
			while (*str == ' ' || *str == '\n' || *str == '\t')
				*str++ = '\0';     
			*argv++ = str;          
			while (*str != '\0' && *str != ' ' && *str != '\t' && *str != '\n') 
				str++;             
		}
		*argv = '\0';                 
	}