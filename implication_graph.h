#ifndef IMPLICATION_GRAPH_H
#define IMPLICATION_GRAPH_H

#include "formula.h"

typedef struct implication_graph_node_t
{
    unsigned depth;
    formula_t *formula;

    int *assignments;
    size_t num_assignments;

    struct implication_graph_node_t **parents;
    size_t num_parents;

    struct implication_graph_node_t **children;
    size_t num_children;
} implication_graph_node_t;

/**
 Initialises the given implication graph node.

 @param formula The formula this implication graph is associated to.
 @param depth The depth at which this node sits in the graph.
*/
void implication_graph_node_init(implication_graph_node_t *node, formula_t *formula, unsigned depth);

/**
 Add a child to the given implication graph node.

 @param variable The variable assignment we are adding to the child.
*/
void implication_graph_node_add_child(implication_graph_node_t *node, int variable);

/**
 Deletes the given implication graph node. This removes the node from its parents' children.
*/
void implication_graph_node_delete(implication_graph_node_t *node);

#define ASSIGNMENT_NOT_FOUND 0

/**
 Finds the assignement of the given variable along the path from the given node to the graph's root.

 @param variable The variable assignement to search for.
 @return The assignment if it exists, ASSIGNMENT_NOT_FOUND otherwise. Negative values indicate negative assignments.
*/
int implication_graph_find_assignment(implication_graph_node_t *node, int variable);

#endif // !IMPLICATION_GRAPH_H
