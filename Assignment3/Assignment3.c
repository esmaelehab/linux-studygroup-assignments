#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mycommand.h"

#define MAX_LIMIT 50
#define OTHER     0x11
#define EXIT      0x12
#define RAND      0x13
#define FACT      0x14
#define FIB       0x15
#define NO        0x16

int checkCommand(char inCommand[]);

char exitCommand[MAX_LIMIT] = "exit";
char randCommand[MAX_LIMIT] = "rand";
char factCommand[MAX_LIMIT] = "fact";
char fibCommand[MAX_LIMIT] = "fib";
char noCommand[MAX_LIMIT] = "";

int main(int argc, char **argv)
{
	char inputCommand[MAX_LIMIT], c;
	int commandId;
	printf("Enter your command > ");
	
	while (1)
	{
		/* clear input command each time */
		for(int i=0; i<MAX_LIMIT; i++)
			inputCommand[i] = 0;

		/* read input command character by character */
		int i = 0;
		while( (c=getchar()) != '\n' )
			inputCommand[i++] = c;
		inputCommand[i] = '\0';

		/* check the input command */
		commandId = checkCommand(inputCommand);

		if ( commandId == OTHER )
                {
                        printf("you said: %s\n", inputCommand);
			printf("enter your command > ");
                }
		else if ( commandId  == EXIT )
		{
			printf("good bye\n");
			return 0;
		}
		else if ( commandId == RAND )
		{
			myrand();
			printf("enter your command > ");
		}
		else if ( commandId == FACT )
		{
			myfact();
			printf("enter your command > ");
		}
		else if ( commandId == FIB )
		{
			myfib();
			printf("enter your command > ");
		}
	}
	return 0;
}

int checkCommand(char inCommand[])
{
	int cmpVal = strcmp(inCommand, exitCommand);
	if(cmpVal == 0)
		return EXIT;

	cmpVal = strcmp(inCommand, randCommand);
	if(cmpVal == 0)
		return RAND;

	cmpVal = strcmp(inCommand, factCommand);
	if(cmpVal == 0)
		return FACT;

	cmpVal = strcmp(inCommand, fibCommand);
	if(cmpVal == 0)
		return FIB;
	
	cmpVal = strcmp(inCommand, noCommand);
	if(cmpVal == 0)
		return NO;

	return OTHER;
}
