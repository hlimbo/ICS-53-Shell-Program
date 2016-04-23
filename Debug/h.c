#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25
#define MAX_ARGS 500

int parse_line(char* buf, char* args[]);

int main(int argc, char * argv[])
{
    while (1)
	{
        printf("prompt> ");
        char line[LENGTH];
       char* args[MAX_ARGS];
//	args[0] = "hello";
//	args[1] = NULL;
		int pid = 0;
		int child_status;
        
		fgets(line, LENGTH, stdin);
		printf("%d\n",strlen(line));
	      parse_line(line, args);
		printf("%s\n",line);
		printf("%s\n",args[0]);
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
	
int  parse_line(char* buf, char* args[]){

	char *delim;
	int argc;
	int bg;

	buf[strlen(buf) - 1] = ' ';
	while(*buf && (*buf == ' '))
		buf++;
	
	argc = 0;
	while ((delim = strchr(buf,' '))) {  
		args[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while(*buf && (*buf == ' '))
			buf++;
	}
	
	args[argc] = NULL;
	if(argc == 0)
		return 1;
	
	if((bg = (*args[argc-1] == '&')) != 0)
		args[--argc] = NULL;
	return bg;
 // char * input = strtok(line, " \n");
   // int i = 0;
   // printf("%s\n",input);
   // strcpy(args[0],input);
}
