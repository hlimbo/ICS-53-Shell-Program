#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500
// FLAGS: 
#define BG 0
#define FG 1
#define INVALID -1

static int enable = 0;
int parse_line(char line[LENGTH], char cache[][LINES], char* args[]);
void signal_handler(int sig);//called asynchronously


int main(int argc, char * argv[])
{
	int pid = 0;
	pid_t wpid;
	int child_status;

	if(signal(SIGCHLD,signal_handler) == SIG_ERR)
		printf("Signal error\n");
	
	while (1)
	{
//		waitpid(-1, &child_status, WNOHANG);
		char line[LENGTH] = {0};
		char cache[LENGTH][LINES];
		char* args[MAX_ARGS];
		
		int n;
		if(enable == 1)
			printf("DREAM> ");
		fgets(line,LENGTH,stdin);
		int value = parse_line(line, cache, args);

	//	if(signal(SIGCHLD,signal_handler) == SIG_ERR)
	//		printf("Signal error: %d, %d\n",SIG_ERR,SIGCHLD);

		if (value == INVALID)
		{
			continue;
		}
		if (strcmp(args[0], "quit") == 0)
		{
			exit(0);
		}
		else//forking
		{
			if ((pid = fork()) == 0)
			{//child process	
				printf("process group in child %d\n",getpgrp());
				if (execvp(args[0], args) < 0)
				{
					printf("\n%s command not found", args[0]);	
					exit(0);
				}
			}
			else
			{//parent process
				if (value == FG)
				{
					printf("Foreground process %d\n",getpid());
					printf("process group in parent %d\n",getpgrp());
					wpid =  wait(&child_status);

					printf("child status %d\n",child_status);
					if(WIFEXITED(child_status))
					{
						printf("Child %d terminated with exit status %d\n",wpid,WEXITSTATUS(child_status));
					}
					else
					{
						printf("Child %d terminate abnormally\n",wpid);
					}
				}
		//		else if (value == BG) 
		//		{
		//			printf("Background process %d\n",getpid());
		//			wpid  = waitpid(pid, &child_status, WNOHANG);
					//bottom does not run
		//			printf("child status %d\n",child_status);
		//			if(WIFEXITED(child_status))
		//			{
		//				printf("Child %d terminated with exit status %d\n",wpid,WEXITSTATUS(child_status));
		//			}
		//			else
		//			{
		//				printf("Child %d terminate abnormally\n",wpid);
		//			}
		//		}
		
				
			}
		}

	}

	return 0;
}

int  parse_line(char line[LENGTH], char cache[][LINES], char* args[])
{
	char * input = strtok(line, " \n\t");
	if (input == NULL)
		return -1;
	int i = 0;
	while (input != NULL)
	{
		strcpy(cache[i], input);
		args[i] = cache[i];
		++i;
		input = strtok(NULL, " \n\t");
	}

	int p_type;

	if (strcmp(args[i - 1], "&") == 0)
	{//background process
		args[i - 1] = NULL;
		p_type = BG;
	}
	else   //foreground process
	{
		args[i] = NULL;
		p_type = FG;
	}

	return p_type;
}

void signal_handler(int sig)
{
	enable = 1;
	//waits for any process
	pid_t pid;
	while((pid = waitpid(-1,NULL,0)) > 0)
	{
		printf("Handler reaped child %d. Received Signal %d\n",(int)pid, sig);
	}
	//ECHILD  no child processes
	if(errno != ECHILD)
	{
		printf("waitpid error\n");
	}
	
//	sleep(2);
//	exit(0);
}

