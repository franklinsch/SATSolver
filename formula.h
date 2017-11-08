#ifndef FORMULA_H
#define FORMULA_H

#include "clause.h"
#include "evaluation.h"
#include "implication_graph.h"
#include "variable_map.h"

typedef struct formula_t
{
    int num_variables;
    size_t num_clauses;
    unsigned _curr_clause;
    clause_t *clauses;
} formula_t;

/**
 Initialise a formula.

 @param num_clauses Number of clauses in the formula.
 @param num_variables Number of variables in the formula.
*/
void formula_init(formula_t *formula, size_t num_clauses, unsigned num_variables);

/**
 Evaluates the given formula given a node in the implication graph. This function
 tries to find assignments for all the variables in each clause by traversing up
 the assignment tree, up to the root.

 @param implication_graph The current implication graph.
 @param unassigned (out) An unassigned literal in the formula.
 @return The result of the evaluation.
 */
EVALUATION formula_evaluate(formula_t *formula, implication_graph_t *implication_graph, int *unassigned);

/**
 Add a clause to the formula.
*/
void formula_add_clause(formula_t *formula, clause_t clause);

/**
 Free resources associated to a formula.
*/
void formula_free(formula_t *formula);

/**
 Print the formula in DIMACS format.
*/
void formula_print(formula_t *formula);

#endif // !FORMULA_H
