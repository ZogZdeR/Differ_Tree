#ifndef GRAPHIC
#define GRAPHIC

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "spusk_func.hpp"

static size_t const system_buffer_size = 35;
static char const *graph_dot = "graph.dot";

void GraphVizDump (node_t *node);
void GraphVizNodeInitialise (FILE *stream, node_t *node);
void GraphVizRoadsInitialise (FILE *stream, node_t *node);

#endif // GRAPHIC