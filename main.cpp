#include "spusk_func.hpp"
#include "graphic.hpp"
#include "diff_func.hpp"

int main ()
{
    char *hui = (char *)"a+1$";
    char **s = &hui;
    node_t *tree_hui = getG (s);
    node_t *diff_hui = Differentiation (tree_hui, "a");
    GraphVizDump (tree_hui);
    GraphVizDump (diff_hui);
    Destructor (tree_hui);
    Destructor (diff_hui);
    return 0;
}