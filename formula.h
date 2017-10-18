#ifndef FORMULA_H
#define FORMULA_H

typedef struct
{
    unsigned clauses;
    int variables
} formula_t;

formula_t *alloc_formula(unsigned clauses, unsigned variables);

void add_clause(formula_t *formula, int *variables);

void dealloc_formula(formula_t *formula);

void print_formula(formula_t *formula);

#endif // !FORMULA_H
