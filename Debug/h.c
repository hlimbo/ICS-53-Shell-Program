#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 80
#define LINES 25

void parse_line(char line[LENGTH], char args[LENGTH][500]);

int main(int argc, const char * argv[])
{
    while (1)
	{
        printf("prompt> ");
        char line[LENGTH];
        char args[LENGTH][500] = {0};
		int pid = 0;
		int child_status;
        
		fgets(line, LENGTH, stdin);
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
				printf("FINISHED FORKING...\n");
				printf("NOW EXITING...\n");
				printf("childs pid: %d\n",pid);


				if((pid = fork()) == 0)
				{
					printf("Inner fork\n");
					printf("Now leaving grandchild\n");
					printf("grandchilds pid: %d\n",pid);
				}
				else
				{
					pid_t wpid = wait(&child_status);
					printf("Reaped the grandchild\n");
					printf("child's  wpid: %d\n",wpid);
				}

				exit(0);
			}
			else
			{
				pid_t wpid = wait(&child_status);
				printf("REAPED THE CHILD...\n");
				printf("%d\n",getpid());
				printf("parents pid: %d\n",wpid);
			}
	
		}
        
	}
    
    return 0;    
 }
	

void parse_line(char line[LENGTH], char args[LENGTH][500]){
    char * input = strtok(line, " \t");
    int i = 0;
    while (input != NULL){
        strcpy(args[i], input);
        ++i;
        input = strtok(NULL, " \t");
    }
}
