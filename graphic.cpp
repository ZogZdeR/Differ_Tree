#include "graphic.hpp"

char const *graph_dot = "graph.dot";

void GraphVizNodeInitialise (FILE *stream, node_t *node)
{
    assert (stream);
    assert (node);

    if (node->left != NULL) 
        GraphVizNodeInitialise (stream, node->left);
    switch (node->node_type)
    {
        case CONST:
            fprintf (stream, "\"Node%p\" [shape = record, style = \"filled\",fillcolor=\"lightgrey\" label = \" { %lld | { <f2> Left | <f3> Right } } \"];\n ", 
                node, node->node_value.value);
            break;
        case VARIABLE:
            fprintf (stream, "\"Node%p\" [shape = record, style = \"filled\",fillcolor=\"lightgrey\" label = \" { %s | { <f2> Left | <f3> Right } } \"];\n ", 
                node, node->node_value.variable);
            break;
        case OPERATOR:
            fprintf (stream, "\"Node%p\" [shape = record, style = \"filled\",fillcolor=\"lightgrey\" label = \" { %s | { <f2> Left | <f3> Right } } \"];\n ", 
                node, functions_structure[node->node_value.oper - 1].name);
            break;
        default:
            assert (0);
            break;
    };
    if (node->right != NULL) 
        GraphVizNodeInitialise (stream, node->right);
}

void GraphVizRoadsInitialise (FILE *stream, node_t *node)
{
    assert (stream);
    assert (node);

    if (node != NULL)
    {
        if (node->left != NULL) 
            GraphVizRoadsInitialise (stream, node->left);
        if (node->left != NULL) 
            fprintf (stream, "\"Node%p\":<f2> -> \"Node%p\" [weight = 1, color = red]; \n", 
                node, node->left);
        if (node->right != NULL) 
            fprintf (stream, "\"Node%p\":<f3> -> \"Node%p\" [weight = 1, color = red]; \n", 
            node, node->right);
        if (node->right != NULL) 
            GraphVizRoadsInitialise (stream, node->right);
    }
}

void GraphVizDump (node_t *node)
{
    assert (node != NULL);

    FILE *dot_file_stream = fopen (graph_dot, "w"); 
    assert (dot_file_stream != NULL);

    static size_t dump_quantity = 0;

    fprintf (dot_file_stream, "digraph {\n");
    fprintf (dot_file_stream, "nodestep = 1;\n");
    fprintf (dot_file_stream, "splines = ortho;\n");

    GraphVizNodeInitialise (dot_file_stream, node);

    fprintf (dot_file_stream, "\n");

    GraphVizRoadsInitialise (dot_file_stream, node);
    fprintf (dot_file_stream, "\n}");

    fclose(dot_file_stream);

    char *some_buffer = (char *)calloc (system_buffer_size, sizeof (char));
    assert (some_buffer != NULL);

    dump_quantity++;
    snprintf (some_buffer, system_buffer_size, "dot -Tpng graph.dot -o graph%zu.png", dump_quantity);
    system (some_buffer);
    free (some_buffer);
}
