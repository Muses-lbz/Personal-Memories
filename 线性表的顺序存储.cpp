#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE	100		//线性表储存空间的初始分配量
#define LISTINCREMENT	10		//线性表储存空间的分配增量
#define TRUE		1
#define FALSE		0
#define OK			1
#define ERROR		0
#define OVERFLOW	-1
typedef int Status;
typedef struct {
	int* elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList* L) {
	//构造一个空的线性表
	L->elem = (int*)malloc(LIST_INIT_SIZE * sizeof(int));
	if (!L->elem)exit(OVERFLOW);
	L->length = 0;
	L->listsize = LIST_INIT_SIZE;
	return OK;
}// InitList_Sq
Status ListLength(SqList L) {
	//返回L中元素的个数
	return L.length;
}
Status ListInsert_Sq(SqList* L, int i, int e) {
	//在顺序线性表L中第i个位置之前插入新的元素e,
	//i的合法值为1<=i<=ListLength_Sq(L)+1
	if (i<1 || i>L->length + 1)return ERROR;	//i值不合法
	if (L->length >= L->listsize) {	//当前存储空间已满，增加分配
		int* newbase = (int*)realloc(L->elem,
			(L->listsize + LISTINCREMENT) * sizeof(int));
		if (!newbase)exit(OVERFLOW);
		L->elem = newbase;
		L->listsize += LISTINCREMENT;
	}
	int* q = &(L->elem[i - 1]);//q为插入位置
	for (int* p = &(L->elem[L->length - 1]); p >= q; --p)*(p + 1) = *p;
	//插入位置及之后的元素右移
	*q = e;//插入e
	++L->length;//增加表长1
	return OK;
}// ListInsert_Sq
void MergeList_Sq(SqList La, SqList Lb, SqList* Lc) {
	//已知顺序线性表La和Lb的元素按值非递减排列
	//归并La和Lb得到新的顺序线性表Lc,Lc的元素也按值非递减排列
	int* pa = La.elem;
	int* pb = Lb.elem;
	Lc->length = La.length + Lb.length;
	Lc->listsize = Lc->length;
	Lc->elem = (int*)malloc(Lc->listsize * sizeof(int));
	int* pc = Lc->elem;
	if (!Lc->elem)exit(OVERFLOW);//储存分配失败
	int* pa_last = La.elem + La.length - 1;
	int* pb_last = Lb.elem + Lb.length - 1;
	while (pa <= pa_last && pb <= pb_last) {	//归并
		if (*pa <= *pb)*pc++ = *pa++;
		else *pc++ = *pb++;
	}
	while (pa <= pa_last)*pc++ = *pa++;//插入La的剩余元素
	while (pb <= pb_last)*pc++ = *pb++;//插入Lb的剩余元素
}// MergeList_Sq

int main()
{
	SqList La;
	SqList Lb;
	SqList Lc;
	InitList_Sq(&La);
	InitList_Sq(&Lb);
	InitList_Sq(&Lc);

	printf("先向线性表La中插入元素，选择插入元素数目：");
	int a1 = 0;
	scanf_s("%d", &a1);
	printf("请输入元素：");
	for (int i = 0; i < a1; i++) {
		scanf_s("%d", &La.elem[i]);
		ListInsert_Sq(&La, i + 1, La.elem[i]);
	}

	printf("再向线性表Lb中插入元素，选择插入元素数目：");
	int a2 = 0;
	scanf_s("%d", &a2);
	printf("请输入元素：");
	for (int i = 0; i < a2; i++) {
		scanf_s("%d", &Lb.elem[i]);
		ListInsert_Sq(&Lb, i + 1, Lb.elem[i]);
	}

	MergeList_Sq(La, Lb, &Lc);
	printf("归并前La表的元素：");
	for (int i = 0; i < La.length; i++)
	{
		printf("%d ", La.elem[i]);
	}
	printf("\n归并前Lb表的元素：");
	for (int i = 0; i < Lb.length; i++)
	{
		printf("%d ", Lb.elem[i]);
	}
	printf("\n归并后Lc表的元素：");
	for (int i = 0; i < Lc.length; i++)
	{
		printf("%d ", Lc.elem[i]);
	}
	return 0;
}
