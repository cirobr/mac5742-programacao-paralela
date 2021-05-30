#include <stdio.h>

/*
A pointer is a variable that holds a memory address
Without the asterisk, the pointer variable represents a memory location
With the asterisk, the pointer represents the value at that location
*/

int main()
{
    char *string = "I'm just a normal string.\n";   //pointer declaration
    puts(string);                                   //strings are vectors of memory locations

    while(*string)
    {
        putchar(*string);                           //displays one character at a time
        string++;                                   // increment memory location
    }
    return 0;
}
