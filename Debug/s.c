#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500
#define SIGINT 2
#define SIGCHLD 18
#define SIGKILL 9

int parse_line(char line[LENGTH], char cache[][LINES], char* args[]);

void bg_handler(int sig) {
	printf("Process %d received signal %d\n", getpid(), sig);
	int stat, pid;
	waitpid(-1, stat, WNOHANG);

	printf("CHILD: %s : REAPED\n", pid);
}

int main(int argc, char * argv[])
{
	signal(SIGCHLD, bg_handler);
	//signal(SIGKILL, bg_handler);
	while (1)
	{
		printf("prompt> ");
		char line[LENGTH];
		char cache[LENGTH][LINES];
		char* args[MAX_ARGS];
		int pid = 0;
		int child_status;
		int bg;
		int bg_pid = 0;

		fgets(line, LENGTH, stdin);
		if ( (bg = parse_line(line, cache, args)) == -1)
		{
			continue;
		}
		if (strncmp(args[0], "quit", 4) == 0)
		{
			return 0;
		}
		else if (bg == 0)
		{
			//setpgid(0,0);
			if ((bg_pid = fork()) == 0)
			{
				if (execvp(args[0], args) < 0 && execv(args[0], args) < 0)
				{
					printf("\nINVALID COMMAND: %s", args[0]);
				}
				printf("\nEXITING CHILD: %d", getpid());
				printf("\nPID: %d\n", bg_pid);
				exit(0);
			}
			else
			{
				printf("\nNO REAP YET\n");
			}
		}
		else//forking
		{
			if ((pid = fork()) == 0)
			{
				if (execvp(args[0], args) < 0 && execv(args[0], args) < 0)
				{
					printf("\nINVALID COMMAND: %s", args[0]);
				}
				printf("\nEXITING CHILD: %d", getpid());
				printf("\nPID: %d\n", pid);
				exit(0);
			}
			else
			{
				int childid = wait(&child_status);
				printf("\nREAPED CHILD: %d\n",childid);
			}
		}

	}

	return 0;
}
int  parse_line(char line[LENGTH], char cache[][LINES], char* args[]) {

	char * input = strtok(line, " \n\t");
	if (input == NULL)
		//null
		return -1;
	int i = 0;
	while (input != NULL) {
		strcpy(cache[i], input);
		args[i] = cache[i];
		++i;
		input = strtok(NULL, " \n\t");
	}
	//background
	if (strcmp(args[i - 1], "&") == 0) {
		args[i - 1] = NULL;
		return 0;
	}
	args[i] = NULL;
	//foreground
	return 1;
}

void cleanup()
{
	printf("Exiting Child");
}

