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
    fprintf (stderr, "meeeeeeeeeew\n");
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
                        return DiffLin (origin);
                        break;
                    case SUB:
                        return DiffLin (origin);
                        break;
                    case MUL:
                        return DiffMul (origin);
                        break;
                    case DIV:
                        return DiffDiv (origin);
                        break;
                    case EXP:
                        return DiffExp (origin);
                        break;
                    case SIN:
                        return DiffSin (origin);
                        break;
                    case COS:
                        return DiffCos (origin);
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

node_t *DiffLin (node_t *original_node)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    if (original_node->node_value.oper == ADD)
    {
        current_node->node_type = OPERATOR;
        current_node->node_value.oper = ADD;
        current_node->left = Differentiation (original_node->left, NONE);
        current_node->right = Differentiation (original_node->right, NONE);
    }
    else
    {
        current_node->node_type = OPERATOR;
        current_node->node_value.oper = SUB;
        current_node->left = Differentiation (original_node->left, NONE);
        current_node->right = Differentiation (original_node->right, NONE);
    }
    return current_node;
}

node_t *DiffMul (node_t *original_node)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = ADD;
    current_node->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->left->node_type = OPERATOR;
    current_node->left->node_value.oper = MUL;
    current_node->left->parent = NULL;
    current_node->left->left = NodeCopy (original_node->left);
    current_node->left->right = Differentiation (original_node->right, NONE);

    current_node->right = (node_t *)calloc (1, sizeof (node_t));
    current_node->right->node_type = OPERATOR;
    current_node->right->node_value.oper = MUL;
    current_node->right->parent = NULL;
    current_node->right->left = Differentiation (original_node->left, NONE);
    current_node->right->right = NodeCopy (original_node->right);
    return current_node;
}

node_t *DiffDiv (node_t *original_node)
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
    new_root->left->left->left = Differentiation (original_node->left, NONE);
    new_root->left->left->right = NodeCopy (original_node->right);

    new_root->left->right = (node_t *)calloc (1, sizeof (node_t));
    new_root->left->right->node_type = OPERATOR;
    new_root->left->right->node_value.oper = MUL;
    new_root->left->right->parent = NULL;
    new_root->left->right->left = Differentiation (original_node->right, NONE);
    new_root->left->right->right = NodeCopy (original_node->left);
    return new_root;
}

node_t *DiffSin (node_t *original_node)
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

    current_node->right = Differentiation (original_node->right, NONE);
    return current_node;
}

node_t *DiffCos (node_t *original_node)
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
    current_node->right->right = Differentiation (original_node->right, NONE);

    current_node->right->left = (node_t *)calloc (1, sizeof (node_t));
    current_node->right->left->node_type = OPERATOR;
    current_node->right->left->node_value.oper = SIN;
    current_node->right->left->parent = NULL;
    current_node->right->left->left = NULL;
    current_node->right->left->right = NodeCopy (current_node->right);
    return current_node;
}

node_t *DiffExp (node_t *original_node)
{
    node_t *current_node = (node_t *)calloc (1, sizeof (node_t));
    current_node->node_type = OPERATOR;
    current_node->node_value.oper = MUL;
    current_node->parent = NULL;
    current_node->left = NodeCopy (original_node);
    current_node->right = Differentiation (original_node->right, NONE);
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