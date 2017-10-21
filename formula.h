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

formula_t *alloc_formula(unsigned num_clauses, unsigned num_variables);

void add_clause(formula_t *formula, clause_t clause);

void dealloc_formula(formula_t *formula);

void print_formula(formula_t *formula);

#endif // !FORMULA_H
