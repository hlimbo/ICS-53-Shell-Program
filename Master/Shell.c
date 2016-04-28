#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500
// FLAGS: 
#define BG 0
#define FG 1
#define INVALID -1

int parse_line(char line[LENGTH], char cache[][LINES], char* args[]);

int main(int argc, char * argv[])
{
	int pid = 0;
	int child_status;
	while (1)
	{
		waitpid(-1, &child_status, WNOHANG);
		printf("Dream> ");
		char line[LENGTH];
		char cache[LENGTH][LINES];
		char* args[MAX_ARGS];

		fgets(line, LENGTH, stdin);
		int value = parse_line(line, cache, args);

		if (value == INVALID)
			continue;

		if (strcmp(args[0], "quit") == 0)
		{
			return 0;
		}
		else//forking
		{
			if ((pid = fork()) == 0)
			{
				if (execvp(args[0], args) < 0 && execv(args[0], args) < 0)
				{
					printf("%s command not found", args[0]);
					exit(0);
				}
			}
			else
			{
				printf("hello from the parent\n");
				if (value == FG)
					wait(&child_status);
				else if (value == BG) {
					printf("Running in background ");
					int return_value = waitpid(pid, &child_status, WNOHANG);
					if (return_value < 0)
						printf("Error: %s", strerror(return_value));
				}
				printf("CT: child has terminated\n");
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

