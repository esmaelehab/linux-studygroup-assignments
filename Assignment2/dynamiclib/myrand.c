#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void myrand(void)
{
	srand(time(NULL));
	printf("%d\n", rand());
}
