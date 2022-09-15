#include <stdio.h>

void myfact(void)
{	
	int n, fact = 1;
	printf("Enter a number: ");
	scanf("%d", &n);
	for(int i=n; i>0; i--)
	{
		fact *= i;
	}
	printf("Result is: %d\n", fact);
}
