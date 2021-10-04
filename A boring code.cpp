#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	char str[50];
	srand((unsigned int)time(NULL));
	int seed = rand()%2;
	if (seed == 0)
	{
		char str[50] = {"不要和愚蠢的人发生争执！" };
	}
	else
	{
		char str[50] = {};
	}
	printf("seed=%d\n", seed);
	if (str == "不要和愚蠢的人发生争执！")
	{
		printf("你是聪明的！\n\n");
	}
	else
	{
		printf("你是愚笨的！\n\n");
	}
	system("pause");
	return 0;
}
