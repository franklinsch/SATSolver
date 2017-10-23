#ifndef FORMULA_H
#define FORMULA_H

#include "clause.h"

typedef struct formula
{
    int num_variables;
    unsigned num_clauses;
    unsigned _curr_clause;
    clause_t *clauses;
} formula_t;

/**
 Initialise a formula

 @param formula pointer to the formula struct
 @param num_clauses number of clauses in the formula
 @param num_variables number of variables in the formula
*/
void formula_init(formula_t *formula, unsigned num_clauses, unsigned num_variables);

/**
 Add a clause to the formula

 @param formula pointer to the formula struct
 @param clause clause to add to the formula
*/
void formula_add_clause(formula_t *formula, clause_t clause);

/**
 Free ressources associated to a formula

 @param formula pointer to the formula struct
*/
void formula_free(formula_t *formula);

/**
 Print the formula in dimacs format

 @param formula pointer to the formula struct
*/
void formula_print(formula_t *formula);

#endif // !FORMULA_H
