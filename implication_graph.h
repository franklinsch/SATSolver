#ifndef IMPLICATION_GRAPH_H
#define IMPLICATION_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct implication_graph_node_t implication_graph_node_t;

typedef struct
{
    implication_graph_node_t *nodes;
    implication_graph_node_t *conflict_node;
    size_t num_variables;
} implication_graph_t;

/**
 Initializes the given implication graph with num_variables.
 */
void implication_graph_init(implication_graph_t *implication_graph, size_t num_variables);

/**
 Frees the resources associated with the given implication graph.
 */
void implication_graph_free(implication_graph_t *implication_graph);

typedef struct clause_t clause_t;

/**
 Adds an assignment to the given implication graph.

 @param assignment The assignment to add.
 @param parent_assignment The assignment which derived assignment. 0 if the assignment was an arbitrary decision.
 @param parent_clause The clause from which assignment was deduced. NULL if the assignment was an arbitrary decision.
 @return Whether adding the assignment causes a conflict.
 */
bool implication_graph_add_assignment(implication_graph_t *implication_graph, int assignment, size_t decision_level, int parent_assignment, clause_t *parent_clause);

/**
 Removes the given assignment from the implication graph
 */
void implication_graph_remove_assignment(implication_graph_t *implication_graph, int assignment);

#define ASSIGNMENT_NOT_FOUND 0

/**
 Finds the given variable's assignment in the implication graph.

 @param variable The variable to look up.
 @return The value of the assignment. ASSIGNMENT_NOT_FOUND if the variable hasn't been assigned.
 */
int implication_graph_find_assignment(implication_graph_t *implication_graph, int variable);

/**
 Removes the given decision variable, as well as the assignments which were deduced from it.
 */
void implication_graph_remove_decision_variable(implication_graph_t *implication_graph, int assignment);

#endif // !IMPLICATION_GRAPH_H
