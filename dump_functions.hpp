#ifndef DUMP_FUNCTIONS
#define DUMP_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "string_functions.hpp"
#include "spusk_func.hpp"


const char LATEX_COMAND[][40] =
{
    "+",
    "-",
    "\\cdot",
    "\\frac",
    "\\^",
    "\\sin",
    "\\cos",
    "\\ln",
    "\\exp",
    "\\tan",
    "\\cot",
    "\\arcsin",
    "\\arccos",
    "\\arctan",
};

int DumpToLaTex (node_t* node);
int DumpNodeLaTex (node_t* node, FILE* stream);
int CleanLaTex ();

#endif // DUMP_FUNCTIONS
