#include "spusk_func.hpp"
#include "graphic.hpp"
#include "diff_func.hpp"
#include "dump_functions.hpp"

int main ()
{
    char const *hui = "COS(15+2*x)$";
    char const **s = &hui;
    node_t *tree_hui = getG (s);
    GraphVizDump (tree_hui);
    // GraphVizDump (tree_hui);
    node_t *diff_hui = Differentiation (tree_hui, "x");
    GraphVizDump (diff_hui);
    Simplifier (diff_hui);
    DumpToLaTex (diff_hui);
    GraphVizDump (diff_hui);
    Destructor (tree_hui);
    Destructor (diff_hui);
    return 0;
}