#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char * argv[] )
{
	if ( argv[1] == NULL )
	{
		printf("mycp: error: no enough arguments\n");
		return 0;
	}

	if ( argv[3] != NULL )
	{
		printf("mycp: error: too many arguments\n");
		return 0;
	}

	char buf[100];
	int destFd = open(argv[2], O_CREAT | O_WRONLY, 0664);
	int srcFd = open(argv[1], O_RDONLY);
	
	if (srcFd == -1)
	{
		printf("mycp: error: no such file exists\n");
		return 0;
	}

	int readCount;
	
	while(readCount = read(srcFd, buf, 100))
	{
		write(destFd, buf, readCount);
	}
	
	close(srcFd);
	close(destFd);
}
