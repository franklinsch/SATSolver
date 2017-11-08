#include "dpll.h"

#include "clause.h"
#include "formula.h"
#include "implication_graph.h"
#include "bcp.h"
#include "variable_map.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define CHOOSE_VAR_ALL_ASSIGNED 0

static int choose_var(int num_variables, implication_graph_t *implication_graph)
{
    for (int i = 1; i <= num_variables; i++)
    {
        if (implication_graph_find_assignment(implication_graph, i) == ASSIGNMENT_NOT_FOUND)
        {
            return i;
        }
    }

    return CHOOSE_VAR_ALL_ASSIGNED;
}

char *tabulate(size_t depth)
{
    char *s = malloc(sizeof (char) * (depth + 1));
    memset(s, ' ', depth);
    s[depth] = '\0';
    return s;
}

EVALUATION _dpll(formula_t *formula, implication_graph_t *implication_graph, int last_assignment, size_t decision_level)
{
    bcp(implication_graph, last_assignment, decision_level);

//    int *end = node->assignments + node->num_assignments;
//    for (int *curr = node->assignments; curr < end; curr++)
//    {
//        variable_map_add(assignment_mirror, *curr, (void *) *curr);
//    }

    int unassigned_lit = 0;
    EVALUATION evaluation = formula_evaluate(formula, implication_graph, &unassigned_lit);

    if (evaluation == EVALUATION_UNDETERMINED)
    {
        int variable = unassigned_lit ? unassigned_lit : choose_var(formula->num_variables, implication_graph);

        // At least one variable should be unassigned, otherwise formula_evaluate would not
        // have returned EVALUATION_UNDETERMINED.
        assert(variable != CHOOSE_VAR_ALL_ASSIGNED);

        // Create a new assignment setting the variable to the positive value.
        implication_graph_add_assignment(implication_graph, variable, decision_level + 1, 0, NULL);

#ifdef DEBUG
        fprintf(stderr, "%sVariable: %d\n", tabulate(decision_level), variable);
#endif
        evaluation = _dpll(formula, implication_graph, variable, decision_level);

        if (evaluation == EVALUATION_FALSE)
        {
#ifdef DEBUG
            fprintf(stderr, "%sBacktrack: %d\n", tabulate(decision_level + 1), variable);
#endif
            // Remove the assignment made previously.

            implication_graph_remove_decision_variable(implication_graph, variable);

            // Create a new assignment with the negated value and run DPLL again.
            implication_graph_add_assignment(implication_graph, -variable, decision_level + 1, 0, NULL);

            EVALUATION other_evaluation = _dpll(formula, implication_graph, -variable, decision_level);

            if (other_evaluation == EVALUATION_FALSE) {
                implication_graph_remove_decision_variable(implication_graph, -variable);
#ifdef DEBUG
                fprintf(stderr, "%sBacktrack: %d\n", tabulate(decision_level + 1), -variable);
#endif
            }
            return other_evaluation;
        }

        return evaluation;
    }

    return evaluation;
}

static void flatten_assignments(implication_graph_t *implication_graph, bool assignments[])
{
    // Set all the values of assignments to DPLL_ASSIGNMENT_DONT_CARE.
    size_t num_variables = implication_graph->num_variables;
    memset(assignments, DPLL_ASSIGNMENT_DONT_CARE, sizeof (bool) * (num_variables));

    for (int i = 1; i <= num_variables; i++)
    {
        int assignment = implication_graph_find_assignment(implication_graph, i);
        if (assignment != ASSIGNMENT_NOT_FOUND)
        {
            assignments[i - 1] = assignment > 0 ? true : false;
        }
    }
}

bool dpll(formula_t *formula, bool assignments[])
{
    implication_graph_t implication_graph;
    implication_graph_init(&implication_graph, formula->num_variables);

    EVALUATION evaluation = bcp_init(formula, &implication_graph);

    if (evaluation == EVALUATION_UNDETERMINED)
    {
        evaluation = _dpll(formula, &implication_graph, 0, 0);
    }

    if (evaluation == EVALUATION_TRUE) flatten_assignments(&implication_graph, assignments);

    implication_graph_free(&implication_graph);
    return evaluation == EVALUATION_TRUE ? true : false;
}
