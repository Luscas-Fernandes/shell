#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int stringHandleStyle(int argc, char **argv, FILE** archive);
void runningShell(char string[], int executionStyle, FILE *archive);
void getString(char string[], int executionStyle, FILE *archive);


int main(int argc, char **argv)
{
    char commands[BUFFER_COMMANDS][BUFFER_SINGLE_COMMAND];
    char string[BUFFER_COMMANDS * BUFFER_SINGLE_COMMAND] = "";

    static char isShellRunning = TRUE;
    static char executionStyle = SEQUENTIAL_RUN;

    FILE *archive;

    executionStyle = stringHandleStyle(argc, argv, &archive);
    
/*     while(isShellRunning)
    {
        runningShell(string, executionStyle, archive);
        } */
    runningShell(string, executionStyle, archive);

    return 0; // explicit return 0
}


void runningShell(char string[], int executionStyle, FILE *archive)
{
    getString(string, executionStyle, archive);
}

int stringHandleStyle(int argc, char **argv, FILE** archive)
{
    if(argc == 1)
        return INTERACTIVE;

    else if(argc == 2)
    {
        *archive = fopen(argv[1], "r");
        if (*archive == NULL)
        {
            fprintf(stderr, "Unable to open file, closing!\n");
            exit(EXIT_FAILURE);
        }

        return BATCH;
    }
    else
    {
        fprintf(stderr, "Too many arguments!\n");
        exit(EXIT_FAILURE);
        return -1;
    }
}

void getString(char string[], int executionStyle, FILE *archive)
{
    char currentLine[BUFFER_COMMANDS * BUFFER_SINGLE_COMMAND];

    if(executionStyle == INTERACTIVE)
    {
        fgets(string, BUFFER_COMMANDS * BUFFER_SINGLE_COMMAND + 1, stdin); // + 1, '\0'
        
        if(feof(stdin)) 
        {
            fprintf(stdout, "Exiting via \"CTRL+D\"\n");
            exit(EXIT_SUCCESS);
        }
    }

    else if(executionStyle == BATCH) // takes only first string. Get whole file
    {
        int stringSizeAccumulator = 0;
        int currentStringSize;
        string[0] = '\0';

        while(fgets(currentLine, BUFFER_COMMANDS * BUFFER_SINGLE_COMMAND + 1, archive) != NULL) // + 1, '\0'
        {
            if(currentLine[0] == '\n' || currentLine[0] == '\0')
                continue;

            currentLine[strcspn(currentLine, "\r\n")] = '\0';

            currentStringSize = strlen(currentLine);

            if(currentLine[currentStringSize - 1] != ';')
                strcat(currentLine, ";");

            currentStringSize = strlen(currentLine);
            stringSizeAccumulator += currentStringSize;

            if(stringSizeAccumulator < BUFFER_SINGLE_COMMAND - 1)
                strcat(string, currentLine);
            else
            {
                fprintf(stdout, "Exiting, the string with commands is bigger than max buffer!\n");
                exit(EXIT_FAILURE);
            }
        }

        string[stringSizeAccumulator + 1] = '\0';

        fprintf(stdout, "%s\n", string);
    }
}

/* void getCommands(char string[], char commands[][])
{

} */