#include "diff_func.hpp"

node_t *NodeCopy (node_t *origin)
{
    node_t *new_root = (node_t *)calloc (1, sizeof(node_t));
    new_root->parent = NULL;
    new_root->node_type = origin->node_type;
    switch (origin->node_type)
    {
        case OPERATOR:
            new_root->node_value.oper = origin->node_value.oper;
            break;
        case CONST:
            new_root->node_value.value = origin->node_value.value;
            break;
        case VARIABLE:
        {
            char *new_var = (char *)calloc (30, 1);
            MyStrcpy (origin->node_value.variable, new_var);
            new_root->node_value.variable = new_var;
            break;
        }
        default:
            assert (0);
            break;
    }
    if (origin->left != NULL) new_root->left = NodeCopy (origin->left);
    else new_root->left = NULL;
    if (origin->right != NULL) new_root->right = NodeCopy (origin->right);
    else new_root->right = NULL;
    return new_root;

}
node_t *Differentiation (node_t *origin, char const *diff_var)
{
    assert (origin != NULL);
    assert (diff_var != NULL);

    switch (origin->node_type)
    {
        case CONST:
            return DiffConst();
            break;
        case VARIABLE:
            return DiffVar (origin, diff_var);
            break;
        case OPERATOR:
            {
                switch (origin->node_value.oper)
                {
                    case ADD:
                        return DiffLin (origin, diff_var);
                        break;
                    case SUB:
                        return DiffLin (origin, diff_var);
                        break;
                    case MUL:
                        return DiffMul (origin, diff_var);
                        break;
                    case DIV:
                        return DiffDiv (origin, diff_var);
                        break;
                    case EXP:
                        return DiffExp (origin, diff_var);
                        break;
                    case SIN:
                        return DiffSin (origin, diff_var);
                        break;
                    case COS:
                        return DiffCos (origin, diff_var);
                        break;
                    default:
                        assert (0);
                }
                break;
            }
        default:
            assert (0);
            break;
    }
} 

node_t *DiffLin (node_t *original_node, char const *diff_var)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    if (original_node->node_value.oper == ADD)
    {
        current_node->node_type = OPERATOR;
        current_node->node_value.oper = ADD;
        current_node->left = Differentiation (original_node->left, diff_var);
        current_node->right = Differentiation (original_node->right, diff_var);
    }
    else
    {
        current_node->node_type = OPERATOR;
        current_node->node_value.oper = SUB;
        current_node->left = Differentiation (original_node->left, diff_var);
        current_node->right = Differentiation (original_node->right, diff_var);
    }
    return current_node;
}

node_t *DiffMul (node_t *original_node, char const *diff_var)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = ADD;
    current_node->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->left->node_type = OPERATOR;
    current_node->left->node_value.oper = MUL;
    current_node->left->parent = NULL;
    current_node->left->left = NodeCopy (original_node->left);
    current_node->left->right = Differentiation (original_node->right, diff_var);

    current_node->right = (node_t *)calloc (1, sizeof (node_t));
    current_node->right->node_type = OPERATOR;
    current_node->right->node_value.oper = MUL;
    current_node->right->parent = NULL;
    current_node->right->left = Differentiation (original_node->left, diff_var);
    current_node->right->right = NodeCopy (original_node->right);
    return current_node;
}

node_t *DiffDiv (node_t *original_node, char const*diff_var)
{
    assert (original_node != NULL);

    node_t *new_root = (node_t *)calloc (1, sizeof (node_t));
    new_root->node_type = OPERATOR;
    new_root->node_value.oper = DIV;
    new_root->parent = NULL;
    new_root->right = (node_t *)calloc (1, sizeof (node_t));
    new_root->left = (node_t *)calloc (1, sizeof (node_t));
    new_root->right->node_type = OPERATOR;
    new_root->right->node_value.oper = MUL;
    new_root->right->parent = NULL;
    new_root->right->right = NodeCopy (original_node->right);
    new_root->right->left = NodeCopy (original_node->right);

    new_root->left->node_type = OPERATOR;
    new_root->left->node_value.oper = SUB;
    new_root->left->parent = NULL;

    new_root->left->left = (node_t *)calloc (1, sizeof (node_t));
    new_root->left->left->node_type = OPERATOR;
    new_root->left->left->node_value.oper = MUL;
    new_root->left->left->parent = NULL;
    new_root->left->left->left = Differentiation (original_node->left, diff_var);
    new_root->left->left->right = NodeCopy (original_node->right);

    new_root->left->right = (node_t *)calloc (1, sizeof (node_t));
    new_root->left->right->node_type = OPERATOR;
    new_root->left->right->node_value.oper = MUL;
    new_root->left->right->parent = NULL;
    new_root->left->right->left = Differentiation (original_node->right, diff_var);
    new_root->left->right->right = NodeCopy (original_node->left);
    return new_root;
}

node_t *DiffSin (node_t *original_node, char const*diff_var)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = MUL;

    current_node->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->left->node_type = OPERATOR;
    current_node->left->node_value.oper = COS;
    current_node->left->parent = NULL;
    current_node->left->right = NodeCopy (original_node->right);
    current_node->left->left = NULL;

    current_node->right = Differentiation (original_node->right, diff_var);
    return current_node;
}

node_t *DiffCos (node_t *original_node, char const *diff_var)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = MUL;
    
    current_node->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->left->node_type = CONST;
    current_node->left->node_value.value = -1;
    current_node->left->parent = NULL;
    current_node->left->right = NULL;
    current_node->left->left = NULL;

    current_node->right = (node_t *)calloc (1, sizeof (node_t));
    current_node->right->node_type = OPERATOR;
    current_node->right->node_value.oper = MUL;
    current_node->right->parent = NULL;
    current_node->right->right = Differentiation (original_node->right, diff_var);

    current_node->right->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->right->left->node_type = OPERATOR;
    current_node->right->left->node_value.oper = SIN;
    current_node->right->left->parent = NULL;
    current_node->right->left->left = NULL;
    current_node->right->left->right = NodeCopy (current_node->right);
    return current_node;
}

node_t *DiffExp (node_t *original_node, char const *diff_var)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = MUL;
    current_node->parent = NULL;
    current_node->left = NodeCopy (original_node);
    current_node->right = Differentiation (original_node->right, diff_var);
    return current_node;
}

node_t *DiffVar (node_t *original_node, char const *differ_var)
{
    if (MyStrncmp (original_node->node_value.variable, differ_var, MyStrlen (differ_var)) != 0) 
    {
        fprintf (stderr, "%s\n", original_node->node_value.variable);
        return NodeCopy (original_node);
    }
    else 
    {
        node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
        current_node->node_type = CONST;
        current_node->node_value.value = 1;
        current_node->parent = NULL;
        current_node->left = NULL;
        current_node->right = NULL;
        return current_node;
    }
}

node_t *DiffConst ()
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = CONST;
    current_node->node_value.value = 0;
    current_node->parent = NULL;
    current_node->left = NULL;
    current_node->right = NULL;
    return current_node;
}


void Simplifier (node_t *node)
{
    size_t beginning_nodes_quantity = 0;
    size_t ending_nodes_quantity = 0;
    bool is_changed = true;
    while (is_changed)
    {
        beginning_nodes_quantity = NodesCounter (node);
        SimplifiersTool (node);
        ending_nodes_quantity = NodesCounter (node);
        if (ending_nodes_quantity == beginning_nodes_quantity) is_changed = false;
    }
}

size_t NodesCounter (node_t *node)
{
    size_t quantity = 1;
    if (node->left != NULL) quantity += NodesCounter (node->left);
    if (node->right != NULL) quantity += NodesCounter (node->right);
    return quantity;
}


void SimplifiersTool (node_t *node)
{
    assert (node != NULL);

    if (node->left != NULL) SimplifiersTool (node->left);
    if (node->node_type == OPERATOR)
    {
        switch (node->node_value.oper)
        {
            case ADD:
                SimplifyAdd (node);
                break;
            case SUB:
                SimplifySub (node);
                break;
            case MUL:
                SimplifyMul (node);
                break;
            case DIV:
                SimplifyDiv (node);
                break;
        }
    }
    if (node->right != NULL) SimplifiersTool (node->right);
}

void SimplifyAdd (node_t *node)
{
    if (node->left->node_type == CONST && node->right->node_type == CONST)
    {
        data_t new_value = node->right->node_value.value + node->left->node_value.value;
        free (node->right);
        free (node->left);
        node->node_type = CONST;
        node->node_value.value = new_value;
        node->left = NULL;
        node->right = NULL;
    }
    else if (node->right->node_type == CONST && node->right->node_value.value == 0)
    {
        free (node->right);
        node_t *new_r = node->left->right;
        node_t *new_l = node->left->left;
        node->node_type = node->left->node_type;
        node->node_value = node->left->node_value;
        free (node->left);
        node->left = new_l;
        node->right = new_r;
    }
    else if (node->left->node_type == CONST && node->left->node_value.value == 0)
    {
        free (node->left);
        node_t *new_r = node->right->right;
        node_t *new_l = node->right->left;
        node->node_type = node->right->node_type;
        node->node_value = node->right->node_value;
        free (node->right);
        node->left = new_l;
        node->right = new_r;
    }
}

void SimplifySub (node_t *node)
{
    if (node->left->node_type == CONST && node->right->node_type == CONST)
    {
        data_t new_value = node->left->node_value.value - node->right->node_value.value;
        free (node->right);
        free (node->left);
        node->node_type = CONST;
        node->node_value.value = new_value;
        node->right = NULL;
        node->left = NULL;
    }
    else if (node->right->node_type == CONST && node->right->node_value.value == 0)
    {
        free (node->right);
        node_t *new_r = node->left->right;
        node_t *new_l = node->left->left;
        node->node_type = node->left->node_type;
        node->node_value = node->left->node_value;
        free (node->left);
        node->left = new_l;
        node->right = new_r;
    }
    else if (node->left->node_type == CONST && node->left->node_value.value == 0)
    {
        node->node_value.oper = MUL;
        node->left->node_value.value = -1;
    }
}

void SimplifyMul (node_t *node)
{
    if (node->left->node_type == CONST && node->right->node_type == CONST)
    {
        data_t new_value = node->left->node_value.value * node->right->node_value.value;
        free (node->right);
        free (node->left);
        node->node_type = CONST;
        node->node_value.value = new_value;
        node->right = NULL;
        node->left = NULL;
    }
    else if (node->right->node_type == CONST && node->right->node_value.value == 1)
    {
        free (node->right);
        node_t *new_r = node->left->right;
        node_t *new_l = node->left->left;
        node->node_type = node->left->node_type;
        node->node_value = node->left->node_value;
        free (node->left);
        node->left = new_l;
        node->right = new_r;
    }
    else if (node->left->node_type == CONST && node->left->node_value.value == 1)
    {
        free (node->left);
        node_t *new_r = node->right->right;
        node_t *new_l = node->right->left;
        node->node_type = node->right->node_type;
        node->node_value = node->right->node_value;
        free (node->right);
        node->left = new_l;
        node->right = new_r;
    }
    else if ((node->left->node_type == CONST && node->left->node_value.value == 0) || 
        (node->right->node_type == CONST && node->right->node_value.value == 0))
    {
        if (node->left->node_type == VARIABLE) free ((char *)node->left->node_value.variable);
        free (node->left);
        if (node->right->node_type == VARIABLE) free ((char *)node->right->node_value.variable);
        free (node->right);
        node->node_type = CONST;
        node->node_value.value = 0;
        node->right = NULL;
        node->left = NULL;
    }
}

void SimplifyDiv (node_t *node)
{
    if (node->left->node_type == CONST && node->right->node_type == CONST)
    {
        data_t new_value = node->left->node_value.value / node->right->node_value.value;
        free (node->right);
        free (node->left);
        node->node_type = CONST;
        node->node_value.value = new_value;
        node->right = NULL;
        node->left = NULL;
    }
    else if (node->right->node_type == CONST && node->right->node_value.value == 1)
    {
        free (node->right);
        node_t *new_r = node->left->right;
        node_t *new_l = node->left->left;
        node->node_type = node->left->node_type;
        node->node_value = node->left->node_value;
        free (node->left);
        node->left = new_l;
        node->right = new_r;
    }
}