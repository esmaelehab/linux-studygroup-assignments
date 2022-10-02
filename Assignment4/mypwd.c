#include <stdio.h>
#include <unistd.h>

int main()
{
	char *buf = NULL;
	size_t size = 5;
	char *path = getcwd(buf, size);
	while ( path == NULL )
	{
		size *= 2;
		path = getcwd(buf, size);
	}
	printf("%s\n", path);
}
