
#include "calculator.hpp"

int main ()
{
    char *hui = "10*(30+20*10)+13$";
    char **s = &hui;
    fprintf (stderr, "%d\n", getG(s));

    return 0;
}