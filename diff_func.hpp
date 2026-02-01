#ifndef DIFF_FUNC
#define DIFF_FUNC

#include "spusk_func.hpp"
#include "string_functions.hpp"

static char const *NONE = "NONE";

node_t *NodeCopy (node_t *origin);
node_t *Differentiation (node_t *origin, char const *diff_var);
node_t *DiffLin (node_t *original_node);
node_t *DiffMul (node_t *original_node);
node_t *DiffDiv (node_t *original_node);
node_t *DiffSin (node_t *original_node);
node_t *DiffCos (node_t *original_node);
node_t *DiffExp (node_t *original_node);
node_t *DiffVar (node_t *original_node, char const *differ_var);
node_t *DiffConst ();




#endif // DIFF_FUNC