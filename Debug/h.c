#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500

void parse_line(char line[LENGTH], char* args[]);

int main(int argc, char * argv[])
{
    while (1)
	{
        printf("prompt> ");
        char line[LENGTH];
       char* args[MAX_ARGS];
		int pid = 0;
		int child_status;
        
		fgets(line, LENGTH, stdin);
		printf("%d\n",strlen(line));
	//	line[strlen(line) - 1] = 0;
	      parse_line(line, args);
	
	   if (strcmp(args[0], "\n") == 0)
	   {
            continue;
      	    }
        else if (strncmp(args[0], "quit", 4) == 0)
		{
            //quit ish
            return 0;
        }
		else//forking
		{
			if ((pid = fork()) == 0)
			{
				if(execv(args[0],args) < 0)
				{
					printf("%s command not found",args[0]);
					exit(0);
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
	
void parse_line(char line[LENGTH], char* args[]){
    char * input = strtok(line, " \n");
    int i = 0;
    printf("%s\n",input);
    strcpy(args[0],input);
}
