#include <stdio.h>

void myfib(void)
{
	int n, res, num1=0, num2=1;
	printf("Enter a number: ");
	scanf("%d", &n);
	printf("The sequence is: ");
	for(int i=0; i<=n; i++)
	{
		printf("%d ", num1);
		res = num1 + num2;
		num1 = num2;
		num2 = res;
	}
	printf("\n");
}
