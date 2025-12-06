#include "diff_func.hpp"
#include "extraction.hpp"


node_t *ReadNode (FILE *stream, char *buffer, size_t * pos)
{
    // char *buffer = (char *)calloc (file_length (stream), sizeof (char));
    // file_to_buffer (stream, buffer);
    char *temp = (char *)calloc (MAX_OPERATOR_SIZE, sizeof (char));
    sscanf (buffer, "%s", temp);
}

node_t *CreateNode (node_t *parent, node_t *left, node_t *right, type new_node_type, data new_node_value)
{
    node_t *new_node = (node_t *)calloc (1, sizeof (node_t));
    new_node->left = NULL;
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->node_type = new_node_type;
    new_node->node_value = new_node_value;
    return new_node;
}

node_t *getP (char **s)
{
    node_t *value = NULL;
    if (**s == '(')
    {
        (*s)++;
        value = getE (s);
        if (**s != ')') assert(0);
        (*s)++;
        return value;
    }
    else return getN (s);
}

node_t *getT (char **s)
{

    node_t *value = getP(s);
    while (**s == '*' || **s == '/')
    {
        int op = **s;
        (*s)++;
        node_t *value_2 = getP (s);
        if (op == '*') 
        {
            data oper;
            oper.op_str = MUL; 
            node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        }
        else
        {
            data oper;
            oper.op_str = DIV; 
            node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        }
    }
    return value;
}

node_t *getE (char **s)
{
    node_t *value = getT(s);
    fprintf(stderr, "GetE outer value from GetT", value);
    while (**s == '+' || **s == '-')
    {
        fprintf(stderr, "GetE: %p\n", *s);
        int op = **s;
        (*s)++;
        node_t *value_2 = getT (s);
        if (op == '+')
        {
            data oper;
            oper.op_str = ADD; 
            node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        }
        else
        {
            data oper;
            oper.op_str = SUB; 
            node_t *new_node = CreateNode (NULL, value, value_2, OPERATOR, oper);
        }
    }
    return value;
}

node_t *getN (char **s)
{

    fprintf (stderr, "GetN outer: %p\n", *s);
    bool found_any = false;
    int value = 0;
    while (**s >= '0' && **s <= '9')
    {
        found_any = true;
        value = value *10 + (**s - '0');
        (*s)++;
        fprintf (stderr, "GetN while: %p\n", *s);
    }
    if (!found_any) assert(0);
    node_t *new_node = (node_t *)calloc (1, sizeof (node_t));
    new_node->left = NULL;
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->node_type =  CONST;
    new_node->node_value.value = value;
    fprintf (stderr, "mew\n");
    return new_node;
}

node_t *getG (char **s)
{
    node_t *value = getE(s);
    fprintf(stderr, "GetG value: %d\n", value);
    if (**s != '$') assert(0);
    return value;
}



