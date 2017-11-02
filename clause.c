#include "clause.h"

#include "variable_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clause_init(clause_t *clause)
{
    variable_vector_init(&clause->variables);
}


EVALUATION clause_evaluate(clause_t *clause, implication_graph_node_t *node)
{
    EVALUATION evaluation = EVALUATION_FALSE;

    for (int *it = variable_vector_cbegin(&clause->variables); it < variable_vector_cend(&clause->variables); it++)
    {
        int assignment_value = implication_graph_find_assignment(node, *it);

        if (*it == assignment_value)
        {
            return EVALUATION_TRUE;
        }
        else if (assignment_value == ASSIGNMENT_NOT_FOUND)
        {
            evaluation = EVALUATION_UNDETERMINED;
        }
    }

    return evaluation;
}

void clause_add_var(clause_t *clause, int var)
{
    variable_vector_push_back(&clause->variables, var);
}

void clause_delete_var(clause_t *clause, size_t index)
{
    variable_vector_delete(&clause->variables, index);
}

int clause_get_var(clause_t *clause, size_t index)
{
    int *res = variable_vector_get(&clause->variables, index);
    return *res;
}

int clause_get_unassigned_literal(const clause_t *clause, implication_graph_node_t *curr_assignment)
{
    for (int *it = variable_vector_cbegin(&clause->variables); it < variable_vector_cend(&clause->variables); it++)
    {
        int ass = implication_graph_find_assignment(curr_assignment, *it);
        if (ass == ASSIGNMENT_NOT_FOUND) return *it;
    }
    return CLAUSE_ASSIGNED;
}

void clause_free(clause_t *clause)
{
    variable_vector_free(&clause->variables);
}
