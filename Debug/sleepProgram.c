#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Incorrect arguments\n");
        return 0;
    }

    int time = atoi(argv[1]);

    sleep(time);
    printf("Slept for %d seconds\n", time);

    return 0;
}
