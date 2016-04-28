#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500

int parse_line(char line[LENGTH], char cache[][LINES], char* args[]);

int main(int argc, char * argv[])
{
    while (1)
	{
        printf("prompt> ");
        char line[LENGTH];
        char cache[LENGTH][LINES];
       char* args[MAX_ARGS];
//	args[0] = "hello";
//	args[1] = NULL;
		int pid = 0;
		int child_status;
        
		fgets(line, LENGTH, stdin);
        if (strcmp(line, "\n") == 0){
            continue;
        }
        parse_line(line, cache, args);
		printf("%s\n",line);
		printf("%s\n",args[0]);
        if (strncmp(args[0], "quit", 4) == 0)
		{
            //quit ish
            return 0;
        }
		else//forking
		{
			if ((pid = fork()) == 0)
			{
				if(execvp(args[0],args) < 0)
				{
					if(execv(args[0],args) < 0)
					{	
						printf("%s command not found",args[0]);
						exit(0);
					}
				}
				printf("FINISHED FORKING...\n");
				printf("NOW EXITING...\n");
				printf("childs pid: %d\n",pid);
			}
			else
			{
				printf("hello from the parent\n");
				wait(&child_status);
				printf("CT: child has terminated\n");
			}
		}
        
	}
    
    return 0;    
 }
	
int  parse_line(char line[LENGTH], char cache[][LINES], char* args[]){

    char * input = strtok(line, " \t");
       int i = 0;
        while (input != NULL){
                strcpy(cache[i], input);
                args[i] = cache[i];
                ++i;
                input = strtok(NULL, " \t");
        }
    
    return 1;
}
