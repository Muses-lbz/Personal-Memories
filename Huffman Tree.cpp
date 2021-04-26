#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>

typedef char TElemType;

typedef struct /* 哈夫曼树结点的结构 */
{
    TElemType data;
    int weight; 
    int lchild; 
    int rchild;
    int parent;
}HuffNode;

typedef struct/* 哈夫曼编码的存储结构 */
{
    TElemType cd[50];
    int start;
}HuffCode;

int HuffmanCreate(HuffNode* h)
{
    int i, k, n, x1, x2, y1, y2;
    printf("请输入元素个数为：");
    scanf("%d", &n);
    for (i = 1; i < n + 1; i++) 
    {
        getchar(); /* 接收回车 */
        printf("第%d个元素的=>\n\t结点值：", i);
        scanf("%c", &h[i].data);
        printf("\t权  值：");
        scanf("%d", &h[i].weight);
    }
    for (i = 1; i <= 2 * n - 1; i++) /* 对数组初始化 */
        h[i].parent = h[i].lchild = h[i].rchild = 0;
    for (i = n + 1; i <= 2 * n - 1; i++)
    {
        x1 = x2 = 999;
        y1 = y2 = i;
        for (k = 1; k < i; k++)
        {
            if (h[k].parent == 0)
            {
                if (h[k].weight < x1) 
                {
                    x2 = x1;
                    y2 = y1;
                    x1 = h[k].weight;
                    y1 = k;
                }
                else if (h[k].weight < x2)
                {
                    x2 = h[k].weight;
                    y2 = k;
                }
            }
        }
        h[y1].parent = i;
        h[y2].parent = i;
        h[i].weight = x1 + x2;
        h[i].lchild = y1;
        h[i].rchild = y2;
    }
    printf("Success For HuffmanTree!\n");
    return n;/*返回结点个数*/
}

void Encoding(HuffNode h[], HuffCode co[], int n)
{
    HuffCode d;
    int i, k, f, c;
    for (i = 1; i < n + 1; i++)
    {
        d.start = n + 1;/*起始位置*/
        c = i;/*从叶结点开始向上*/
        f = h[i].parent;
        while (f != 0)
        {
            if (h[f].lchild == c)
                d.cd[--d.start] = '0';
            else
                d.cd[--d.start] = '1';
            c = f;
            f = h[f].parent;
        }
        co[i] = d;
    }
    printf("输出哈夫曼编码：\n");
    for (i = 1; i <= n; i++)
    {
        printf("%c:", h[i].data);
        for (k = co[i].start; k <= n; k++)
        {
            printf("%c", co[i].cd[k]);
        }
        printf("\n");
    }
}

int main()
{
    int flag = 5, c;
    while (flag) {
        printf("\n***************************电文的编码***************************\n");
        printf("\n*********1.建立哈夫曼树        2.编码        0.退出系统*********\n");
        printf("\n----------------------------------------------------------------\n");
        scanf("%d", &flag);
        if (flag == 1)
        {
            HuffNode h[200];
            HuffCode co[100];
            int n; //节点数
            n = HuffmanCreate(h);
            scanf("%d", &c);
            while (c)/* 选择功能 */
            {
                if (c == 2)
                {
                    Encoding(h, co, n);
                }
                if (c == 0)
                {
                    printf("退出系统。\n");
                    break;
                }
                scanf("%d", &c);
            }
        }
        else if (flag == 0)
        {
            printf("退出系统。\n");
            break;
        }
        else
        {
            printf("输入错误，请重新输入！\n");
        }
    }
    return 0;
}