#include <stdio.h>
#include <stdlib.h>

int main()
{
	char x[] = { "I LOVE YOU!" };
	int y;
	scanf_s("%d", &y);
	if (y == 1)
	{
		printf("%s\n", x);
	}
	else
	{
		printf("\n");
	}
	system("pause");
	return 0;
}
