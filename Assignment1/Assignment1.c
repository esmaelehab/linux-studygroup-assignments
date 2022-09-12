
#include <stdio.h>
#include <string.h>

#define MAX_LIMIT 50

int main(int argc, char **argv)
{
	char inputCommand[MAX_LIMIT];
	char exitCommand[MAX_LIMIT] = "exit";
	int cmpValue;
	while (1)
	{
	    printf("enter your command > ");
	    scanf("%[^\n]%*c", inputCommand);
	   
	    cmpValue = strcmp(inputCommand, exitCommand);
	    if ( cmpValue == 0 )
	    {
			printf("good bye\n");
			return 0;
		}
		else
		{	
			printf("you said: %s\n", inputCommand);
	    }
	}
	return 0;
}
