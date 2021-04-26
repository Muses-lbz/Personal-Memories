#include<stdio.h>
#include<stdlib.h>
#define TRUE		1
#define FALSE		0
#define OK			1
#define ERROR		0
#define OVERFLOW	-1
typedef struct LNode {
	int data;
	struct LNode* next;
}LNode, * LinkList;
typedef int Status;
LinkList MergeList_L(LinkList La, LinkList Lb) {
	LinkList Lc;
	LNode* pa; LNode* pb; LNode* pc;
	Lc = (LNode*)malloc(sizeof(LNode));
	if (Lc == NULL)exit(OVERFLOW);
	pa = La->next;
	pb = Lb->next;
	pc = Lc;
	while (pa && pb) {
		if (pa->data <= pb->data) {
			LNode* s = (LNode*)malloc(sizeof(LNode));
			if (s == NULL)exit(OVERFLOW);
			s->data = pa->data;
			pc->next = s;
			pc = s;
			pa = pa->next;
		}
		else {
			LNode* s = (LNode*)malloc(sizeof(LNode));
			if (s == NULL)exit(OVERFLOW);
			s->data = pb->data;
			pc->next = s;
			pc = s;
			pb = pb->next;
		}
	}
	while (pa != NULL) {
		LNode* s = (LNode*)malloc(sizeof(LNode));
		if (s == NULL)exit(OVERFLOW);
		s->data = pa->data;
		pc->next = s;
		pc = s;
		pa = pa->next;
	}
	while (pb != NULL) {
		LNode* s = (LNode*)malloc(sizeof(LNode));
		if (s == NULL)exit(OVERFLOW);
		s->data = pb->data;
		pc->next = s;
		pc = s;
		pb = pb->next;
	}
	pc->next = NULL;
	printf("归并函数调用!");
	return Lc;
}
LinkList Create_L(void) {
	int len;//单链表的结点数
	int val;//单链表数据域的值
	LNode* Head = (LNode*)malloc(sizeof(LNode));//动态分配头结点的内存
	if (Head == NULL) {
		printf("内存分配失败！\n");
		exit(OVERFLOW);
	}
	LNode* Tail = Head;
	Tail->next = NULL;
	printf("请输入单链表结点的个数：");
	scanf_s("%d", &len);
	for (int i = 0; i < len; i++) {
		printf("请输入第%d个结点的值：", i + 1);
		scanf_s("%d", &val);
		LNode* New = (LNode*)malloc(sizeof(LNode));
		if (New == NULL) {
			printf("内存分配失败！\n");
			exit(OVERFLOW);
		}
		New->data = val;
		New->next = NULL;
		Tail->next = New;
		Tail = New;
	}
	return Head;
}
void Traverse_L(LinkList L) {
	LNode* p = L->next;
	printf("单链表的结点元素为：");
	if (p == NULL) {
		printf("这是空表。\n");
	}
	while (p != NULL) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}
int main() {
	LinkList La = NULL;
	La = Create_L();
	Traverse_L(La);

	LinkList Lb = NULL;
	Lb = Create_L();
	Traverse_L(Lb);

	LinkList Lc = MergeList_L(La, Lb);
	Traverse_L(Lc);
	return 0;
}

