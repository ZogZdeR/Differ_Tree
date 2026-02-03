#include "spusk_func.hpp"
#include "extraction.hpp"


/*node_t *ReadNode (FILE *stream, char *buffer, size_t *pos)
{
    
}*/

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

node_t *getP (char const **s)
{
    node_t *value = NULL;
    if (**s == '(')
    {
        (*s)++;
        value = getE (s);
        if (**s != ')') assert (0);
        (*s)++;
        return value;
    }
    else return getF (s);
}

node_t *getT (char const **s)
{

    node_t *value = getP(s);
    while (**s == '*' || **s == '/' || **s == '^')
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
            case '^':
                oper.oper = DEG;
                break;
            default:
                assert (0);
        }
        (*s)++;
        node_t *value_2 = getP (s);
        node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        value = new_node;
    }
    return value;
}

node_t *getE (char const **s)
{
    node_t *value = getT(s);
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
        node_t *value_2 = getT (s);
        node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        value = new_node;
    }
    return value;
}

node_t *getN (char const **s)
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

node_t *getG (char const **s)
{
    node_t *value = getE(s);
    if (**s != '$') assert(0);
    return value;
}

node_t *getF (char const **s)
{
    if ((**s >= 'a' && **s <= 'z') || (**s >= 'A' && **s <= 'Z') || (**s == '^'))
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
                    (NULL, NULL, getP (s), OPERATOR, function_realisation);//было гетE
                return new_node;
            }
        }
        return getV (s);
    }
    return getN (s);
}

node_t *getV (char const **s)
{
    char const **beg = s;
    bool found_any = false;
    char *var = (char *)calloc (30, 1);
    if ((**beg >= 'a' && **beg <= 'z') || (**beg >= 'A' && **beg <= 'Z'))
    {
        size_t num = 0;
        found_any = true;
        var[num] = **s;
        num += 1;
        (*s) ++;
        while ((**s >= '0' && **s <= '9') || (**s >= 'a' && **s <= 'z') || (**s >= 'A' && **s <= 'Z'))
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
        return getN (s);
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

