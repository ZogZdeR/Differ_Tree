#include "calculator.hpp"

#include <assert.h>

int getP (char **s)
{
    int value = 0;
    if (**s == '(')
    {
        (*s)++;
        value = getE (s);
        if (**s != ')') assert(0);
        (*s)++;
        return value;
    }
    else return getN (s);
}

int getT (char **s)
{

    int value = getP(s);
    while (**s == '*' || **s == '/')
    {
        int op = **s;
        (*s)++;
        int value_2 = getP (s);
        if (op == '*') value *= value_2;
        else value /= value_2;
    }
    return value;
}

int getE (char **s)
{
    int value = getT(s);
    fprintf(stderr, "GetE outer value from GetT", value);
    while (**s == '+' || **s == '-')
    {
        fprintf(stderr, "GetE: %p\n", *s);
        int op = **s;
        (*s)++;
        int value_2 = getT (s);
        if (op == '+') value += value_2;
        else value -= value_2;
    }
    return value;
}

int getN (char **s)
{

    fprintf (stderr, "GetN outer: %p\n", *s);
    bool found_any = false;
    int value = 0;
    while (**s >= '0' && **s <= '9')
    {
        found_any = true;
        value = value *10 + (**s - '0');
        (*s)++;
        fprintf (stderr, "GetN while: %p\n", *s);
    }
    if (!found_any) assert(0);
    fprintf (stderr, "mew\n");
    return value;
}

int getG (char **s)
{
    int value = getE(s);
    fprintf(stderr, "GetG value: %d\n", value);
    if (**s != '$') assert(0);
    return value;
}
