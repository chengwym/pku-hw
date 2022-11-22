#include<stdio.h>
void scan(int* a1, int* a2, int s, int d);
void bub(int p, int* arr);
void merge(int* arr1, int* arr2, int k, int l);
void output(int* arr3, int M);
int m, n, a[100], b[100];
int main()
{   
    scanf("%d%d", &m, &n);
    scan(a, b, m, n);
    bub(m, a);
    bub(n, b);
    merge(a, b, m, n);
    output(a, m + n);
    return 0;
}
void scan(int* a1, int* a2, int s, int d)//录入数组
{
    for (int i = 0; i < s; i++)
    {
        scanf("%d", (a1 + i));
    }
    for (int i = 0; i < d; i++)
    {
        scanf("%d", (a2 + i));
    }
}
void bub(int p, int* arr)//冒泡排序
{
    int temp;
    for (int i = 1; i < p; i++)
    {
        for (int j = 0; j < p - i; j++)
        {
            if (*(arr + j) > *(arr + j+ 1))
            {
                temp = *(arr + j);
                *(arr + j) = *(arr + j + 1);
                *(arr + j+ 1) = temp;
            }
        }
    }
}
void merge(int* arr1, int* arr2, int k, int l)//数组合并
{
    int* arr3;
    int j = 0;
    for (int i = k; i < k + l; i++)
    {
        *(arr1 + i) = *(arr2 + j);
        j++;
    }
}
void output(int* arr3, int M)//输出数组
{
    printf("%d", *arr3);
    for (int i = 1; i < M; i++)
    {
        printf(" %d", *(arr3+i));
    }
}





