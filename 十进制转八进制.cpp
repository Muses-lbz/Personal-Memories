#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define stack_size 100
#define restack_size 50
typedef struct stack
{
	int* top;
	int* bottom;
	int stacksize;
}sta;
sta buildstack(sta& s)
{
	s.bottom = (int*)malloc(stack_size * sizeof(int));
	if (!s.bottom)
	{
		printf("内存申请失败！");
		exit(0);
	}
	s.top = s.bottom;
	s.stacksize = stack_size;
	return s;
}
sta add(sta& s, int e)
{
	if (s.top - s.bottom >= s.stacksize)
	{
		s.bottom = (int*)realloc(s.bottom, (stack_size + restack_size) * sizeof(int));

		if (!s.bottom)
		{
			printf("内存申请失败！");
			exit(0);
		}
		s.top = s.bottom + s.stacksize;
		s.stacksize = s.stacksize + restack_size;
	}
	*s.top++ = e;
	return s;
}
sta gettop(sta& s, int& e)
{
	if (s.top == s.bottom)
		return s;
	e = *(s.top - 1);
	return s;
}
sta deltop(sta& s, int& e)
{
	if (s.top == s.bottom)
	{
		return s;
	}
	e = *--s.top;
	return s;
}
int empty(sta& s)
{
	if (s.top == s.bottom)
	{
		return 1;
	}
	else
		return 0;
}
int main()
{
	sta s = buildstack(s);
	int N, e;
	printf("输入数值：");
	scanf("%d", &N);
	while (N)
	{
		add(s, N % 8);
		N = N / 8;
	}
	while (!empty(s))
	{
		deltop(s, e);
		printf("%d", e);
	}
}
