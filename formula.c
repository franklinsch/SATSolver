#include "formula.h"
#include "clause.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

formula_t *alloc_formula(unsigned num_clauses, unsigned num_variables)
{
    formula_t *formula = malloc(sizeof (formula_t));

    formula->num_variables = (int) num_variables;
    formula->num_clauses = num_clauses;
    formula->_curr_clause = 0;
    formula->clauses = malloc(num_clauses * sizeof (clause_t));

    return formula;
}

void add_clause(formula_t *formula, clause_t clause)
{
    // Double check that only up to num_clauses clauses are added to formula
    assert(formula->_curr_clause < formula->num_clauses);
    formula->clauses[formula->_curr_clause++] = clause;
}

void dealloc_formula(formula_t *formula)
{
    if (formula)
    {
        // Deallocate the ressources associated with each clause
        clause_t *end = (formula->clauses + formula->num_clauses);
        for (clause_t *c = formula->clauses; c < end; ++c)
        {
            clause_free(c);
        }

        // Deallocate the clauses array
        free(formula->clauses);
        // Deallocate the formula itself
        free(formula);
    }
}

void print_formula(formula_t *formula)
{
    if (formula)
    {
        printf("p cnf %d %u\n", formula->num_variables, formula->num_clauses);
    }
    else
    {
        fprintf(stderr, "%s: The formula is empty.\n", __FUNCTION__);
        return;
    }

    clause_t *end = (formula->clauses + formula->num_clauses);
    for (clause_t *c = formula->clauses; c < end; ++c)
    {
        for (size_t i = 0; i < c->size; ++i)
        {
            int var = clause_get_var(c, i);
            printf("%d ", var);
        }
        printf("\n");
    }
}
