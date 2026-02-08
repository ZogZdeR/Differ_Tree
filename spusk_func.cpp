#include "spusk_func.hpp"
#include "extraction.hpp"

node_t *CreateNode (node_t *parent, node_t *left, node_t *right, type new_node_type, data new_node_value)
{
    node_t *new_node = (node_t *)calloc (1, sizeof (node_t));
    new_node->left = left;
    new_node->parent = parent;
    new_node->right = right;
    new_node->node_type = new_node_type;
    new_node->node_value = new_node_value;
    return new_node;
}

node_t *GetPart (char const **s)
{
    node_t *value = NULL;
    if (**s == '(')
    {
        (*s)++;
        value = GetExpr (s);
        if (**s != ')') assert (0);
        (*s)++;
        return value;
    }
    else return GetFunc (s);
}

node_t *GetDeg (char const **s)
{
    node_t *value = GetPart (s);
    while (**s == '^')
    {
        data oper;
        oper.oper = ADD;
        if (**s == '^') oper.oper = DEG;
        (*s)++;
        node_t *value_2 = GetPart (s);
        node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        value = new_node;
    }
    return value;
}

node_t *GetTerm (char const **s)
{

    node_t *value = GetDeg (s); //P->D
    while (**s == '*' || **s == '/')
    {
        data oper;
        switch (**s)
        {
            case '*':
                oper.oper = MUL;
                break;
            case '/':
                oper.oper = DIV;
                break;
            default:
                assert (0);
        }
        (*s)++;
        node_t *value_2 = GetDeg (s); // P->D
        node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        value = new_node;
    }
    return value;
}

node_t *GetExpr (char const **s)
{
    node_t *value = GetTerm (s);
    while (**s == '+' || **s == '-')
    {
        data oper;
        switch (**s)
        {
            case '+':
                oper.oper = ADD;
                break;
            case '-':
                oper.oper = SUB;
                break;
            default:
                assert (0);
        }
        (*s)++;
        node_t *value_2 = GetTerm (s);
        node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        value = new_node;
    }
    return value;
}

node_t *GetNumb (char const **s)
{
    bool found_any = false;
    int value = 0;
    while (**s >= '0' && **s <= '9')
    {
        found_any = true;
        value = value *10 + (**s - '0');
        (*s)++;
    }
    if (!found_any) assert (0);
    node_t *new_node = (node_t *)calloc (1, sizeof (node_t));
    new_node->left = NULL;
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->node_type =  CONST;
    new_node->node_value.value = value;
    return new_node;
}

node_t *GetGen (char const **s)
{
    node_t *value = GetExpr (s);
    if (**s != '$') assert(0);
    return value;
}

node_t *GetFunc (char const **s)
{
    if (isalpha (**s) || (**s == '^'))
    {
        for (size_t i = (size_t)DEG - 1; i < FUNCTIONS_QUANTITY; i++) //starting from deg cause of other func are already checked
        {
            const char *temp = *s;
            if (MyStrncmp (temp, functions_structure[i].name, (size_t)MyStrlen (functions_structure[i].name)) == 0)
            {
                data function_realisation;
                function_realisation.oper = functions_structure[i].oper_enum;
                (*s) += MyStrlen (functions_structure[i].name);
                node_t *new_node = CreateNode
                    (NULL, NULL, GetPart (s), OPERATOR, function_realisation);//было гетE
                return new_node;
            }
        }
        return GetVar (s);
    }
    return GetNumb (s);
}

node_t *GetVar (char const **s)
{
    bool found_any = false;
    char *var = (char *)calloc (ENOUGH_STRING_SIZE, 1);
    if (isalpha (**s))
    {
        size_t num = 0;
        found_any = true;
        var[num] = **s;
        num += 1;
        (*s) ++;
        while (isdigit (**s) || isalpha (**s))
        {  
            var[num] = **s;
            (*s) ++;
            num ++;
        }
        var = (char *)realloc (var, num + 1); // + 1 для \0
    }
    if (found_any == false)
    {
        free (var);
        return GetNumb (s);
    }
    else
    {
        node_t *new_node = (node_t *)calloc (1, sizeof (node_t));
        new_node->left = NULL;
        new_node->parent = NULL;
        new_node->right = NULL;
        new_node->node_type =  VARIABLE;
        new_node->node_value.variable = var;
        return (new_node);
    }
}

void Destructor (node_t *node)
{
    assert (node != NULL);

    if (node->left != NULL)
    {
        Destructor (node->left);
    }
    if (node->right != NULL)
    {
        Destructor (node->right);
    }
    if (node->node_type == VARIABLE)
    {
        free ((char *)node->node_value.variable);
        node->node_value.variable = NULL;
    }
    free (node);
}

