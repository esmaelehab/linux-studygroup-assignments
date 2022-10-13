#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int parseCmdArgs (char a_inCmd[], char a_inMulArr[][20]);
void cleanArr (char a_inCmd[]);
void storeUserCmd (char a_inCmd[]);
void handleCmd ( char *a_argv[]); 
int checkForVar (char a_inMulArr[][20], int CmdNum);
void setLocalVars (char a_inMulArr[][20], int vars);
int checkVarExist (char a_temp[], char a_varArr[][20], int size);
void handleSetCmd (void);
void handleExportCmd (char a_inMulArr[][20], int a_argc);

extern char **environ;

char exitCommand[100] = "exit";
char *setCommand = "set";
char varNameBase[20][20];
char varValueBase[20][20];
char temp[20];
char *exportCommand = "export";

int varCount;

int main()
{
	char inCmd[100];
       
	while(1)
	{
		char *newargv[20] = {NULL};
		char inMulArr[20][20];
		
		cleanArr(inCmd);
		printf("enter your command > ");
		storeUserCmd(inCmd);
		
		if ( strlen(inCmd) == 0)
			continue;
		if ( strcmp(inCmd, exitCommand) == 0)
			return 0;
		
		 int argc = parseCmdArgs(inCmd, inMulArr);

		 if ( strcmp(inMulArr[0], setCommand) == 0 /*&& strlen(inMulArr[1]) == 0*/)
                 {
			 handleSetCmd();
		 }
		 else if ( strcmp(inMulArr[0], exportCommand) == 0)
                 {
			 handleExportCmd(inMulArr, argc);
                 }
		 else if (checkForVar(inMulArr, argc) == 1)
                 {
                         setLocalVars(inMulArr, argc);
                 }
		 else if (checkForVar(inMulArr, argc) == 1)
                 {
                         setLocalVars(inMulArr, argc);
                 }
                 else if (checkForVar(inMulArr, argc) == -1)
                 {
                         printf("error : wrong syntax to define variables\n");
                 }
		 else
		 {
			 for (int i=0; i<argc; i++)
                                 newargv[i] = inMulArr[i];
                         newargv[argc] = NULL;
                         handleCmd(newargv);
		 }	 
	}
}


void cleanArr (char a_inCmd[])
{
	for (int i=0; i<strlen(a_inCmd); i++)
		a_inCmd[i] = 0;
}


void storeUserCmd (char a_inCmd[])
{
        char c;
        int i = 0;
        while( (c=getchar()) != '\n' )
	        a_inCmd[i++] = c;
        a_inCmd[i] = '\0';
}

void handleSetCmd (void)
{
	for (int i=0; i<varCount; i++)
                                 printf("local_var[%d]: %s = %s\n", i, varNameBase[i], varValueBase[i]);
}


void handleExportCmd (char a_inMulArr[][20], int a_argc)
{
	char *name = NULL;
	char *value = NULL;
	for (int i=1; i<a_argc; i++)
	{
		int ret = checkVarExist(a_inMulArr[i], varNameBase, varCount);
		if (ret == -1)
		{
			printf("error : export : local variable doesn't exist\n");
		}
		else if (ret != -1)
		{
			name = varNameBase[ret];
			value = varValueBase[ret];
			setenv(varNameBase[ret], varValueBase[ret], i);
		}
	}
}


int parseCmdArgs (char a_inCmd[], char a_inMulArr[][20])
{
	int j=0, i=0, k=0;
	for(i=0; i<strlen(a_inCmd); i++)
	{
		if (a_inCmd[i] != ' ')
		{
			a_inMulArr[k][j] = a_inCmd[i];
			j++;
		}
		else if (a_inCmd[i] == ' ' && a_inCmd[i+1] != ' ')
		{
			if (j == 0)
			{
				// do nothing
			}
			else
			{
				a_inMulArr[k][j] = '\0';
				k++;
				j = 0;
			}
		}
	}
	a_inMulArr[k][j] = '\0';
	
	return k + 1;
}

int checkForVar (char a_inMulArr[][20], int CmdNum)
{
        int equalNum = 0;
        for (int i=0; i<CmdNum; i++)
        {
	        for (int j=0; j<strlen(a_inMulArr[i]); j++)
		{
		        if (a_inMulArr[i][j] == '=')
		       	{
			        equalNum++;
		       	}
		}
        }
	if (equalNum == CmdNum)
		return 1;
	else if (equalNum == 0)
		return 0;
	else
		return -1;
}


void setLocalVars (char a_inMulArr[][20], int vars)
{
	for (int i=0; i<vars; i++)
        {
		int ret;
			int count, index=0;
               		for (count=0; a_inMulArr[i][count]!='='; count++)
			{
				temp[index] = a_inMulArr[i][count];
				
				ret = checkVarExist(temp, varNameBase, varCount);
				if (ret != -1)
				{	
					break;
				}
				else if (ret == -1)
				{
					varNameBase[varCount][index] = a_inMulArr[i][count];
					index++;
				}
			}
			
			if (ret != -1)
				varNameBase[varCount][index] = '\0';

			count++;
			index = 0;

			if ( ret != -1)
			{
				count++;
				for (count; a_inMulArr[i][count]!='\0'; count++)
				{
					varValueBase[ret][index] = a_inMulArr[i][count];
					index++;
				}
				varValueBase[ret][index] = '\0';
			}
			else if (ret == -1)
			{
				for (count; a_inMulArr[i][count]!='\0'; count++)
				{
					varValueBase[varCount][index] = a_inMulArr[i][count];
					index++;
				}
				varValueBase[varCount][index] = '\0';
			}
                
		if (ret == -1)
			varCount++;
	}
}


int checkVarExist (char a_temp[], char a_varArr[][20], int size)
{
	for (int i=0; i<size; i++)
	{
		if (strcmp(a_temp, a_varArr[i]) == 0)
			return i;
	}
	return -1;
}


void handleCmd ( char *a_argv[])
{
	int ret_pid = fork();

	if (ret_pid < 0)
		printf("fork failed\n");
	else if (ret_pid > 0)
	{
		int status;
		wait(&status);
	}
	else if (ret_pid == 0)
	{
		execvp(a_argv[0], a_argv);
		printf("exec failed\n");
	}
}
