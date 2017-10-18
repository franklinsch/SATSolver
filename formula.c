#include "formula.h"

#include <stdio.h>
#include <stdlib.h>

formula_t *alloc_formula(unsigned num_clauses, unsigned num_variables)
{
    formula_t *formula = malloc(sizeof (formula_t));

    formula->num_clauses = num_clauses;
    formula->num_variables = (int) num_variables;

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
        printf("p cnf %d %u\n", formula->num_variables, formula->num_clauses);
}
