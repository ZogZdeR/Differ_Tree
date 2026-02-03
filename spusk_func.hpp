#ifndef SPUSK_FUNC
#define SPUSK_FUNC

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "string_functions.hpp"

typedef signed long long int data_t;

static size_t const FUNCTIONS_QUANTITY = 14;

static size_t const MAX_OPER_VALUE = 7;

enum type
{
    CONST = 1,
    VARIABLE = 2,
    OPERATOR = 3
};

enum oper_t
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    DEG = 5,
    SIN = 6,
    COS = 7,
    LN = 8,
    EXP = 9,
    TAN = 10,
    CTAN = 11,
    ASIN = 12,
    ACOS = 13,
    ATAN = 14
};

union data
{
    data_t value;
    char *variable;
    oper_t oper;  
};

struct function_struct_t
{
    char const *name;
    oper_t oper_enum;
};

function_struct_t static functions_structure[FUNCTIONS_QUANTITY] = // is_used, problem of compiler
{
    "+",     ADD,
    "-",     SUB,
    "*",     MUL,
    "/",     DIV,
    "^",     DEG,
    "SIN",   SIN,
    "COS",   COS,
    "LN",     LN,
    "EXP",   EXP,
    "TAN",   TAN,
    "CTAN", CTAN,
    "ASIN", ASIN,
    "ACOS", ACOS,
    "ATAN", ATAN
};


struct node_t
{
    node_t *parent;
    type node_type;
    data node_value;
    node_t *right;
    node_t *left;
};

node_t *CreateNode (node_t *parent, node_t *left, node_t *right, type new_node_type, data new_node_value);
node_t *getN (char const **s);
node_t *getP (char const **s);
node_t *getT (char const **s);
node_t *getE (char const **s);
node_t *getG (char const **s);
node_t *getF (char const **s);
node_t *getV (char const **s);
void Destructor (node_t *node);

#endif // SPUSK_FUNC