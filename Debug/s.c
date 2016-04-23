#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int pid = 0;
	int child_status;
	if ((pid = fork()) == 0)
	{
		printf("FINISHED FORKING...\n");
		printf("NOW EXITING...\n");
		exit(0);
	}
	else
	{
		pid_t wpid = wait(&child_status);
		printf("REAPED THE CHILD...\n");
	}
	return 0;
}
