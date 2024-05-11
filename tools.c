#include "tools.h"

char *fsgets(char *s, int size, char *input)
{
    if (!*input)
    {
        return NULL;
    }
    while (size && *input != '\n' && *input)
    {
        *s = *input;
        ++s;
        ++input;
        --size;
    }
    *s = 0;

    if (*input)
        return input + 1;
    return input;
}

int string_is_empty(char *s)
{
    while (*s != 0)
    {
        if (*s != ' ' && *s != '\n')
        {
            return 0;
        }
        ++s;
    }
    return 1;
}