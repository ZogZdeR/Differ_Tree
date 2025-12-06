#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef signed long long int data_t;


char const ADD[] = "+";
char const SUB[] = "-";
char const MUL[] = "*";
char const DIV[] = "/";
char const DEG[] = "^";
char const LN[] = "ln";
char const SIN[] = "sin";
char const COS[] = "cos";

size_t const MAX_OPERATOR_SIZE = 4;

char const *operators [] =
{
    ADD,
    SUB,
    MUL,
    DIV,
    DEG,
    LN,
    SIN,
    COS
};
union data
{
    data_t value;
    int variable;
    char const *op_str;  
};

enum type
{
    CONST = 1,
    VARIABLE = 2,
    OPERATOR = 3
};

struct node_t
{
    node_t *parent;
    type node_type;
    data node_value;
    node_t *right;
    node_t *left;
};

node_t *getN (char **s);