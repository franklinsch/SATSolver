#include "clause.h"

#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clause_init(clause_t *clause)
{
    vector_init(&clause->variables);
}


EVALUATION clause_evaluate(clause_t *clause, implication_graph_node_t *node, vector_t *unassigned_lits)
{
    EVALUATION evaluation = EVALUATION_FALSE;
    if (unassigned_lits) vector_init(unassigned_lits);

    int bob = 0;

    void **it;
    for (it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        int it_val = (int) *it;
        bob = it_val;
        int assignment_value = implication_graph_find_assignment(node, it_val);

        if (it_val == assignment_value)
        {
            return EVALUATION_TRUE;
        }
        else if (assignment_value == ASSIGNMENT_NOT_FOUND)
        {
            evaluation = EVALUATION_UNDETERMINED;
            if (unassigned_lits) vector_push_back(unassigned_lits, *it);
        }
    }

    it--;
    int ass = implication_graph_find_assignment(node, (int) *it);
    if (bob == -5 && evaluation == EVALUATION_FALSE)
        printf("");
    return evaluation;
}

void clause_add_var(clause_t *clause, int var)
{
    vector_push_back(&clause->variables, (void *) var);
}

void clause_delete_var(clause_t *clause, size_t index)
{
    vector_delete(&clause->variables, index);
}

int clause_get_var(clause_t *clause, size_t index)
{
    int *res = (int *) vector_get(&clause->variables, index);
    return *res;
}

void clause_populate_unassigned_literals(const clause_t *clause,
        implication_graph_node_t *curr_assignment,
        vector_t *unassigned_lits)
{
    vector_init(unassigned_lits);
    for (void **it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        int ass = implication_graph_find_assignment(curr_assignment, (int) *it);
        if (ass == ASSIGNMENT_NOT_FOUND) {
            vector_push_back(unassigned_lits, *it);
        }
    }
}

void clause_free(clause_t *clause)
{
    vector_free(&clause->variables);
}

void clause_print(clause_t *clause)
{
    for (void **it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        printf("%d ", (int) *it);
    }
    printf("\n");
}
