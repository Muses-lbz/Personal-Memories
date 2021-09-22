#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char str[50] = { "不要和愚蠢的人发生争执！" };
	/*puts(str);*/
	if (str == "不要和愚蠢的人发生争执！")
		printf("你是聪明的！\n");
	else
		printf("你是愚笨的！\n");
	system("pause");
	return 0;
}
