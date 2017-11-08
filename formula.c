#include "formula.h"
#include "clause.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void formula_init(formula_t *formula, size_t num_clauses, unsigned num_variables)
{
    formula->num_variables = (int) num_variables;
    formula->num_clauses = num_clauses;
    formula->_curr_clause = 0;
    formula->clauses = malloc(num_clauses * sizeof (clause_t));
}

EVALUATION formula_evaluate(formula_t *formula, implication_graph_t *implication_graph, int *unassigned)
{
    EVALUATION evaluation = EVALUATION_TRUE;

    clause_t *end = formula->clauses + formula->num_clauses;
    *unassigned = 0;
    for (clause_t *curr = formula->clauses; curr < end; curr++)
    {
        EVALUATION curr_evaluation;

        if (!(*unassigned)) {
//            vector_t unassigned_lits;

            static int unassigned_lits[3];
            unassigned_lits[0] = 0;
            unassigned_lits[1] = 0;
            unassigned_lits[2] = 0;
            curr_evaluation = clause_evaluate(curr, implication_graph, unassigned_lits);

            if (unassigned_lits[0] != 0) {
                *unassigned = unassigned_lits[0];
            }

//            vector_free(&unassigned_lits);
        }
        else
        {
            curr_evaluation = clause_evaluate(curr, implication_graph, NULL);
        }

        switch (curr_evaluation)
        {
            // If one of the clauses evaluates to false, the formula is false.
            case EVALUATION_FALSE:
                return EVALUATION_FALSE;

            // If one of the clauses cannot be evaluated, the formula cannot be either.
            case EVALUATION_UNDETERMINED: evaluation = EVALUATION_UNDETERMINED; break;
            default: break;
        }
    }

    return evaluation;
}

void formula_add_clause(formula_t *formula, clause_t clause)
{
    // Double check that only up to num_clauses clauses are added to formula.
    assert(formula->_curr_clause < formula->num_clauses);
    formula->clauses[formula->_curr_clause++] = clause;
}

void formula_free(formula_t *formula)
{
    if (formula)
    {
        // Deallocate the ressources associated with each clause.
        clause_t *end = (formula->clauses + formula->num_clauses);
        for (clause_t *c = formula->clauses; c < end; ++c)
        {
            clause_free(c);
        }

        // Deallocate the clauses array.
        free(formula->clauses);
    }
}

void formula_print(formula_t *formula)
{
#ifdef DEBUG
    if (!formula)
    {
        fprintf(stderr, "%s: The formula is empty.\n", __func__);
        return;
    }

    printf("p cnf %d %zu\n", formula->num_variables, formula->num_clauses);

    clause_t *end = (formula->clauses + formula->num_clauses);
    for (clause_t *c = formula->clauses; c < end; ++c)
    {
        for (size_t i = 0; i < c->variables.size; ++i)
        {
            int var = clause_get_var(c, i);
            printf("%d ", var);
        }
        printf("\n");
    }
#endif
}
