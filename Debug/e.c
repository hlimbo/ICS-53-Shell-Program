//
//  main.c
//  Lab_2
//
//  Created by Eric Franco on 4/22/16.
//  Copyright © 2016 Eric Franco. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define LENGTH 80
#define LINES 25
void parse_line(char line[LENGTH], char args[LENGTH][500]);
int main(int argc, const char * argv[]) {
    while (1){
        printf("prompt> ");
        char line[LENGTH];
        char args[LENGTH][500] = {0};
        fgets(line, LENGTH, stdin);
        parse_line(line, args);
        if (strcmp(args[0], "\n") == 0){
            continue;
        }
        else if (strncmp(args[0], "quit", 4) == 0){
            //quit ish
            return 0;
        }
        //Just an test.
        else if (strcmp(args[0], "print") == 0){
            int i = 0;
            while (strcmp(args[i], "") != 0){
                printf("%s", args[i]);
                ++i;
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