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
        int value = parse_line(line, cache, args);
        //null
        if( value == -1)
            continue;
        int i = 0;
        for (i = 0; args[i] != NULL; ++i){
        printf("%s\n", args[i]);
        }
        //if its 0 its background
        //if (value == 0)
        if (strcmp(args[0], "quit") == 0)
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
				waitpid(pid, &child_status, WNOHANG);
				printf("CT: child has terminated\n");
			}
		}
        
	}
    
    return 0;    
 }
	
int  parse_line(char line[LENGTH], char cache[][LINES], char* args[]){

    char * input = strtok(line, " \n\t");
    if (input == NULL)
        //null
        return -1;
       int i = 0;
        while (input != NULL){
                strcpy(cache[i], input);
                args[i] = cache[i];
                ++i;
                input = strtok(NULL, " \n\t");
        }
    //background
    if (strcmp(args[i - 1], "&") == 0){
        args[i -1] = NULL;
        return 0;
    }
        args[i] = NULL;
    //foreground
    return 1;
}
