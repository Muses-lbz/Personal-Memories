#include<stdio.h>
#define true 1
#define false 0

bool canPlaceFlowers(int* flowerbed, int flowerbedSize, int n)
{
    int count = 0;
    int prev = -1;
    for (int i = 0; i < flowerbedSize; ++i)
    {
        if (flowerbed[i] == 1)
        {
            if (prev < 0)
            {
                count += i / 2;
            }
            else
            {
                count += (i - prev - 2) / 2;
            }
            prev = i;
        }
    }
    if (prev < 0)
    {
        count += (flowerbedSize + 1) / 2;
    }
    else
    {
        count += (flowerbedSize - prev - 1) / 2;
    }
    return count >= n;
}

int main()
{
    //int flowerbed[] = { 1,0,0,0,1 }, flowerbedSize = 5, n = 2;
    int flowerbed[] = { 1,0,0,0,1 }, flowerbedSize = 5, n = 1;
    int index;
    index = canPlaceFlowers(flowerbed, flowerbedSize, n);
    //printf("%d", index);
    if (index == true)
        printf("true\n");
    else if (index == false)
        printf("false\n");
}