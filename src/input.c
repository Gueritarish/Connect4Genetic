#include "input.h"

#include <stdio.h>

int char_to_int(char c)
{
    if (c < '0' || c > '9')
        return -1;
    return c - '0';
}

int gather_input()
{
    char c = EOF;
    char save;

    // Read characters one by one
    save = getchar();
    while(save != '\n')
    {
        c = save;
        save = getchar();
    }
    return char_to_int(c);
}