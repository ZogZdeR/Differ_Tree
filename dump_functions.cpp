#include "dump_functions.hpp"
#include "spusk_func.hpp"


int DumpToLaTex (node_t* node)
{
    assert (node != NULL);
    
    CleanLaTex ();

    FILE* stream = fopen (TEX_FILE, "a");
    assert (stream != NULL);
        
    fprintf (stream, "\\begin{dmath}\n");
    fprintf (stream, "\\text{");
    fprintf (stream, ": } \\frac {dx}{d}(");

    DumpNodeLaTex (node, stream);
    fprintf (stream, ")= 0\n\\end{dmath}\n");

    fclose (stream);

    return 0;
}

int DumpNodeLaTex (node_t* node, FILE* stream)
{
    assert (node);
    assert (stream);

    if (node->node_type == OPERATOR && node->node_value.oper == DIV)
    {
        fprintf (stream, " %s {", LATEX_COMAND[DIV-1]);
        DumpNodeLaTex (node->left, stream);
        fprintf (stream, "}{");
        DumpNodeLaTex (node->right, stream);
        fprintf (stream, "}");
        return 0;
    }

    if (node->left) DumpNodeLaTex (node->left, stream);

    if (node->node_type == OPERATOR) fprintf (stream, " %s ", LATEX_COMAND[node->node_value.oper]);
    else if (node->node_type == VARIABLE) fprintf (stream, " %s ", node->node_value.variable);
    else
    {
        if (node->node_value.value < 0) fprintf (stream, " (%lld) ", node->node_value.value); 
        else fprintf (stream, " %lld ", node->node_value.value);
    }

    if (node->node_type == OPERATOR && node->node_value.value == DEG)
    {
        fprintf (stream, "{");
        DumpNodeLaTex (node->right, stream);
        fprintf (stream, "}");
        return 0;
    }

    if (node->right) DumpNodeLaTex (node->right, stream);
    return 0;
}

int CleanLaTex ()
{
    FILE* stream = fopen (TEX_FILE, "w");
    assert (stream != NULL);

    fprintf (stream,
             "\\documentclass[a4paper,12pt]{article}\n\n"
             "\\usepackage[utf8]{inputenc}\n"
             "\\usepackage[russian]{babel}\n"
             "\\usepackage{amsmath}\n"
             "\\usepackage{breqn}\n"
             "\\usepackage{graphicx}\n"
            //  "\\usepackage{mathtools}\n"
             "\\usepackage{amssymb}\n"
             "\\usepackage{caption}\n"
             "\\begin{document}\n\n");

    fclose (stream);

    return 0;
}
