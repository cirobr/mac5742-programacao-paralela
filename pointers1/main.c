#include <stdio.h>

/*
A pointer is a variable that holds a memory address
Without the asterisk, the pointer variable represents a memory location
With the asterisk, the pointer represents the value at that location
*/

int main()
{
    int pokey;
    printf("The address of 'pokey' is %p \n", &pokey);

    int *p;                                                 // pointer declaration
    p = &pokey;                                             // pokey address is stored on pointer p
    printf("The address of 'pokey' is %p \n", p);           // pointer without the asterisk

    printf("The contents of pokey is %d \n", pokey);
    printf("The contents at location 'p' is %d \n", *p);    // pointer with the asterisk

    return 0;
}
