#include "spusk_func.hpp"
#include "graphic.hpp"
#include "diff_func.hpp"
#include "dump_functions.hpp"

int main ()
{
    char const *hui = "LN(x)$";
    char const **s = &hui;
    node_t *tree_hui = getG (s);
    GraphVizDump (tree_hui);
    // GraphVizDump (tree_hui);
    node_t *diff_hui = Differentiation (tree_hui, "x");
    Simplifier (diff_hui);
    DumpToLaTex (tree_hui);
    DumpToLaTex (diff_hui);
    GraphVizDump (diff_hui);
    Destructor (tree_hui);
    Destructor (diff_hui);
    return 0;
}