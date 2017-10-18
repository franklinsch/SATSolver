#include "formula.h"

#include <stdio.h>
#include <stdlib.h>

formula_t *alloc_formula(unsigned clauses, unsigned variables)
{
    formula_t *formula = malloc(sizeof (formula_t));

    formula->clauses = clauses;
    formula->variables = (int) variables;

    return formula;
}

void add_clause(formula_t *formula, int *variables)
{
    //TODO
    if (variables)
        free(variables);
}

void dealloc_formula(formula_t *formula)
{
    if(formula)
        free(formula);
}

void print_formula(formula_t *formula)
{
    if (formula)
        printf("p cnf %d %u\n", formula->variables, formula->clauses);
}
