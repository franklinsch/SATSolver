#include "formula.h"
#include "clause.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void formula_init(formula_t *formula, unsigned num_clauses, unsigned num_variables)
{
    formula->num_variables = (int) num_variables;
    formula->num_clauses = num_clauses;
    formula->_curr_clause = 0;
    formula->clauses = malloc(num_clauses * sizeof (clause_t));
}

void formula_add_clause(formula_t *formula, clause_t clause)
{
    // Double check that only up to num_clauses clauses are added to formula
    assert(formula->_curr_clause < formula->num_clauses);
    formula->clauses[formula->_curr_clause++] = clause;
}

void formula_free(formula_t *formula)
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
    }
}

void formula_print(formula_t *formula)
{
    if (!formula)
    {
        fprintf(stderr, "%s: The formula is empty.\n", __func__);
        return;
    }

    printf("p cnf %d %u\n", formula->num_variables, formula->num_clauses);

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
