#include <stdio.h>
#include <stdlib.h>

void main()
{
    int count=10;
    int *pt;
    pt=&count;
    printf("%d \n", count);
    printf("%x \n", *pt);
    // printf("%x \n", pt);  //erro de compilação

    return;
}