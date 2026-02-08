#include "spusk_func.hpp"
#include "graphic.hpp"
#include "diff_func.hpp"
#include "dump_functions.hpp"

int main ()
{
    char const *expression = "COS(15+2*x)$";
    char const **s = &expression;
    node_t *tree_expression = GetGen (s);
    GraphVizDump (tree_expression);
    // GraphVizDump (tree_expression);
    node_t *diff_expression = Differentiation (tree_expression, "x");
    GraphVizDump (diff_expression);
    Simplifier (diff_expression);
    DumpToLaTex (diff_expression);
    GraphVizDump (diff_expression);
    Destructor (tree_expression);
    Destructor (diff_expression);
    return 0;
}