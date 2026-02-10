#include "diff_func.hpp"
#include "graphic.hpp"

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
            char *new_var = (char *)calloc (ENOUGH_STRING_SIZE, 1);
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
                    case LN:
                        return DiffLn (origin, diff_var);
                        break;
                    case EXP:
                        return DiffExp (origin, diff_var);
                        break;
                    case DEG:
                        return DiffDeg (origin, diff_var);
                        break;
                    case SIN:
                        return DiffSin (origin, diff_var);
                        break;
                    case COS:
                        return DiffCos (origin, diff_var);
                        break;
                    case TAN:
                        return DiffTan (origin, diff_var);
                        break;
                    case CTAN:
                        return DiffCTan (origin, diff_var);
                        break;
                    case ATAN:
                        return DiffATan (origin, diff_var);
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
    node_t *current_node = NULL;
    if (original_node->node_value.oper == ADD) current_node = OperMaker (ADD, 
        Differentiation (original_node->left, diff_var), Differentiation (original_node->right, diff_var));
    else current_node = OperMaker (DIV, 
        Differentiation (original_node->left, diff_var), Differentiation (original_node->right, diff_var));
    return current_node;
}

node_t *DiffMul (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (ADD, OperMaker (MUL, 
        NodeCopy (original_node->left), Differentiation (original_node->right, diff_var)), 
        OperMaker (MUL, Differentiation (original_node->left, diff_var), NodeCopy (original_node->right)));
    return current_node;
}

node_t *DiffDiv (node_t *original_node, char const *diff_var)
{
    assert (original_node != NULL);

    node_t *current_node = OperMaker (DIV, 
        OperMaker (SUB, 
            OperMaker (MUL, Differentiation (original_node->left, diff_var), NodeCopy (original_node->right)), 
            OperMaker (MUL, Differentiation (original_node->right, diff_var), NodeCopy (original_node->left))), 
        OperMaker (MUL, NodeCopy (original_node->right), NodeCopy (original_node->right)));
    return current_node;
}

node_t *DiffSin (node_t *original_node, char const*diff_var)
{
    node_t *current_node = OperMaker (MUL, 
        OperMaker (COS, NULL, NodeCopy (original_node->right)),
        Differentiation (original_node->right, diff_var));
    return current_node;
}

node_t *DiffCos (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL, 
        ConstMaker (-1), 
        OperMaker (MUL, 
            OperMaker (SIN, NULL, NodeCopy (original_node->right)), 
            Differentiation (original_node->right, diff_var)));
    return current_node;
}

node_t *DiffExp (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL, 
        NodeCopy (original_node),
        Differentiation (original_node->right, diff_var));
    return current_node;
}

node_t *DiffVar (node_t *original_node, char const *differ_var)
{
    if (MyStrncmp (original_node->node_value.variable, differ_var, (size_t)MyStrlen (differ_var)) != 0) 
    {
        return ConstMaker (0);
    }
    else 
    {
        return ConstMaker (1);
    }
}

node_t *DiffConst ()
{
    return ConstMaker (0);
}

node_t *DiffTan (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL,
    OperMaker (DIV,
        ConstMaker (1),
        OperMaker (MUL,
            OperMaker (COS, NULL, NodeCopy (original_node->right)),
            OperMaker (COS, NULL, NodeCopy (original_node->right)))),
        Differentiation (original_node->right, diff_var));
        
    return current_node; 
}

node_t *DiffCTan (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL,
    OperMaker (DIV,
        ConstMaker (-1),
        OperMaker (MUL,
            OperMaker (SIN, NULL, NodeCopy (original_node->right)),
            OperMaker (SIN, NULL, NodeCopy (original_node->right)))),
    Differentiation (original_node->right, diff_var));

    return current_node; 
}

node_t *DiffLn (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL, 
        OperMaker (DIV,
            ConstMaker (1),
            NodeCopy (original_node->right)),
        Differentiation (original_node->right, diff_var));

    return (current_node);
}

node_t *DiffDeg (node_t *original_node, char const *diff_var)
{
    node_t *degree = OperMaker (MUL,
        OperMaker (LN, NULL, NodeCopy (original_node->left)),
        NodeCopy (original_node->right));

    node_t *current_node = OperMaker (MUL,
        NodeCopy (original_node),
        Differentiation (degree, diff_var));
    Destructor (degree);

    return (current_node);
}

node_t *DiffATan (node_t *original_node, char const *diff_var)
{
    node_t *current_node = OperMaker (MUL,
        Differentiation (original_node->right, diff_var),
        OperMaker (DIV,
            ConstMaker (1),
            OperMaker (ADD, 
                ConstMaker (1),
                OperMaker (DEG,
                    NodeCopy (original_node->right),
                    ConstMaker (2)
                )
            )
        )
    );
    return (current_node);
}


node_t *ConstMaker (data_t constant)
{
    node_t *node = (node_t *)calloc (1, sizeof (node_t));
    node->node_type = CONST;
    node->node_value.value = constant;
    node->parent = NULL;
    node->right = NULL;
    node->left = NULL;
    return node;
}

node_t *OperMaker (oper_t oper, node_t *left, node_t *right)
{
    node_t *node = (node_t *)calloc (1, sizeof (node_t));
    node->node_type = OPERATOR;
    node->node_value.oper = oper;
    node->parent = NULL;
    node->right = right;
    node->left = left;
    return node;
}

node_t *VarMaker (char const *var, node_t *left, node_t *right)
{
    node_t *node = (node_t *)calloc (1, sizeof (node_t));
    node->node_type = VARIABLE;
    node->node_value.variable = (char *)calloc (ENOUGH_STRING_SIZE, 1);
    MyStrncpy (var, node->node_value.variable, MyStrlen (var));
    node->parent = NULL;
    node->right = right;
    node->left = left;
    return node;
}

void Simplifier (node_t *node)
{
    assert (node != NULL);

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
    assert (node != NULL);

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
            default:
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
        Destructor (node->right);
        Destructor (node->left);
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

node_t *DifferentiationOfNDegree (node_t *origin, size_t n, char const *diff_var)
{
    assert (origin != NULL);
    assert (diff_var != NULL);

    node_t *safe_to_loose_origin = NodeCopy (origin);
    return DiffN (safe_to_loose_origin, n, diff_var);
}
node_t *DiffN (node_t *origin, size_t n, char const *diff_var)
{
    if (n == 0) return NodeCopy (origin);
    node_t *exect_diff = Differentiation (origin, diff_var);
    Destructor (origin);
    return DiffN (exect_diff, n-1, diff_var);
}