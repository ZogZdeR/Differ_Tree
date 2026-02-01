#ifndef DIFF_FUNC
#define DIFF_FUNC

#include "spusk_func.hpp"
#include "string_functions.hpp"


node_t *NodeCopy (node_t *origin);
node_t *Differentiation (node_t *origin, char const *diff_var);
node_t *DiffLin (node_t *original_node, char const *diff_var);
node_t *DiffMul (node_t *original_node, char const *diff_var);
node_t *DiffDiv (node_t *original_node, char const *diff_var);
node_t *DiffSin (node_t *original_node, char const *diff_var);
node_t *DiffCos (node_t *original_node, char const *diff_var);
node_t *DiffExp (node_t *original_node, char const *diff_var);
node_t *DiffVar (node_t *original_node, char const *differ_var);
node_t *DiffConst ();

void Simplifier (node_t *node);
size_t NodesCounter (node_t *node);
void SimplifiersTool (node_t *node);
void SimplifyAdd (node_t *node);
void SimplifySub (node_t *node);
void SimplifyMul (node_t *node);
void SimplifyDiv (node_t *node);


#endif // DIFF_FUNC