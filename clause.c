#include "clause.h"
#include "implication_graph.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clause_init(clause_t *clause)
{
    vector_init(&clause->variables);
}

EVALUATION clause_evaluate(clause_t *clause, implication_graph_t *implication_graph, int unassigned_lits[3])
{
    EVALUATION evaluation = EVALUATION_FALSE;

    size_t num_assigned = 0;
    for (void **it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        int it_val = (int) *it;
        int assignment_value = implication_graph_find_assignment(implication_graph, it_val);

        if (assignment_value == ASSIGNMENT_NOT_FOUND)
        {
            assignment_value = implication_graph_find_assignment(implication_graph, -it_val);
        }

        if (it_val == assignment_value)
        {
            return EVALUATION_TRUE;
        }
        else if (assignment_value == ASSIGNMENT_NOT_FOUND)
        {
            evaluation = EVALUATION_UNDETERMINED;
            if (unassigned_lits && num_assigned < 3)
            {
                unassigned_lits[num_assigned++] = *(int*) it;
            }
        }
    }

    return evaluation;
}

void clause_add_var(clause_t *clause, int var)
{
    vector_push_back(&clause->variables, (void *) (uintptr_t) var);
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

void clause_populate_unassigned_literals(const clause_t *clause, implication_graph_t *implication_graph, vector_t *unassigned_lits)
{
    vector_init(unassigned_lits);
    int num_unassigned = 0;
    for (void **it = vector_cbegin(&clause->variables); num_unassigned < 3 && it < vector_cend(&clause->variables); it++)
    {
        int ass = implication_graph_find_assignment(implication_graph, (int) *it);
        if (ass == ASSIGNMENT_NOT_FOUND) {
            vector_push_back(unassigned_lits, *it);
            num_unassigned++;
        }
    }
}

void clause_free(clause_t *clause)
{
    vector_free(&clause->variables);
}

void clause_print(clause_t *clause)
{
#ifdef DEBUG
    for (void **it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        fprintf(stderr, "%d ", (int) *it);
    }
    fprintf(stderr, "\n");
#endif
}
