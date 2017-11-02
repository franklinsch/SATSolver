#ifndef CLAUSE_H
#define CLAUSE_H

#include "evaluation.h"
#include "implication_graph.h"
#include "variable_vector.h"

#include <stddef.h>

#define CLAUSE_ASSIGNED 0

typedef struct
{
    variable_vector_t variables;
} clause_t;

/**
 Initialise a clause, and allocate all the necessary resources on the heap.
*/
void clause_init(clause_t *clause);


/**
 Evaluates the given clause given a node in the implication graph. This function
 tries to find assignments for all the variables in the clause by traversing up
 the assignment tree, up to the root.

 @param node The current assignment node. This function traverses its parents.
 @return The result of the evaluation.
 */
EVALUATION clause_evaluate(clause_t *clause, implication_graph_node_t *node);

/**
 Add a variable to the clause. This function does not check if the variable is
 already in the clause. Having the same variable multiple times in the same clause
 does not change its semantics.

 @param var Variable index, negative values indicate a negation.
*/
void clause_add_var(clause_t *clause, int var);

/**
 Remove a variable from the clause.

 @param index The index of the element to delete.
*/
void clause_delete_var(clause_t *clause, size_t index);

/**
 Get a variable from the clause at the given index.

 @param index Index of the element to inspect.
 @return The variable, where a negative value indicates negation.
*/
int clause_get_var(clause_t *clause, size_t index);

/**
 Get an unassigned_literal from the given clause.

 @param curr_assignment The current assignment
 @return An unassigned variable in the clause or CLAUSE_ASSIGNED if there is no such variable.
*/
int clause_get_unassigned_literal(const clause_t *clause, implication_graph_node_t *curr_assignment);

/**
 Deallocate the clause and all associated ressources.
*/
void clause_free(clause_t *clause);

#endif // !CLAUSE_H
