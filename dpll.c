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

struct dpll_result
{
    EVALUATION evaluation;
    implication_graph_node_t *leaf;
};

#define CHOOSE_VAR_ALL_ASSIGNED 0

static int choose_var(int num_variables, implication_graph_node_t *node)
{
    for (int i = 1; i <= num_variables; i++)
    {
        if (implication_graph_find_assignment(node, i) == ASSIGNMENT_NOT_FOUND)
        {
            return i;
        }
    }

    return CHOOSE_VAR_ALL_ASSIGNED;
}

char *tabulate(int depth)
{
    char *s = malloc(sizeof (char) * (depth + 1));
    memset(s, ' ', depth);
    s[depth] = '\0';
    return s;
}

struct dpll_result _dpll(formula_t *formula, implication_graph_node_t *node, variable_map_t *assignment_mirror)
{
    struct dpll_result result;

    bcp(node, assignment_mirror);

//    int *end = node->assignments + node->num_assignments;
//    for (int *curr = node->assignments; curr < end; curr++)
//    {
//        variable_map_add(assignment_mirror, *curr, (void *) *curr);
//    }

    int unassigned_lit = 0;
    EVALUATION evaluation = formula_evaluate(formula, assignment_mirror, &unassigned_lit);

    if (evaluation == EVALUATION_UNDETERMINED)
    {
        int variable = unassigned_lit ? unassigned_lit : choose_var(formula->num_variables, node);

        // At least one variable should be unassigned, otherwise formula_evaluate would not
        // have returned EVALUATION_UNDETERMINED.
        assert(variable != CHOOSE_VAR_ALL_ASSIGNED);

        // Create a new assignment setting the variable to the positive value.
        implication_graph_node_t *child = implication_graph_node_add_child(node, variable);

        variable_map_add(assignment_mirror, variable, (void *) variable);
#ifdef DEBUG
        fprintf(stderr, "%sVariable: %d\n", tabulate(node->depth), variable);
#endif
        result = _dpll(formula, child, assignment_mirror);

        evaluation = result.evaluation;

        if (evaluation == EVALUATION_FALSE)
        {
#ifdef DEBUG
            fprintf(stderr, "%sBacktrack: %d\n", tabulate(node->depth), variable);
#endif
            // Remove the assignment made previously.

            int *end = child->assignments + child->num_assignments;
            for (int *curr = child->assignments; curr < end; curr++)
            {
                variable_map_remove(assignment_mirror, *curr);
            }

            implication_graph_node_delete(child);

            // Create a new assignment with the negated value and run DPLL again.
            child = implication_graph_node_add_child(node, -variable);

            variable_map_add(assignment_mirror, -variable, (void *) -variable);

            struct dpll_result other_result = _dpll(formula, child, assignment_mirror);
            if (other_result.evaluation == EVALUATION_FALSE) {
                int *end = child->assignments + child->num_assignments;
                for (int *curr = child->assignments; curr < end; curr++)
                {
                    variable_map_remove(assignment_mirror, *curr);
                }
#ifdef DEBUG
                fprintf(stderr, "%sBacktrack: %d\n", tabulate(node->depth), -variable);
#endif
            }
            return other_result;
        }

        return result;
    }

    result.evaluation = evaluation;
    result.leaf = node;
    return result;
}

static void flatten_assignments(implication_graph_node_t *leaf, bool assignments[])
{
    // Set all the values of assignments to DPLL_ASSIGNMENT_DONT_CARE.
    memset(assignments, DPLL_ASSIGNMENT_DONT_CARE, sizeof (bool) * (leaf->formula->num_variables));

    // At the end of DPLL, the graph will contain a single path from the root to the last assignment.
    // Every node will have at most one parent, so we pick it.
    for (implication_graph_node_t *node = leaf; node != NULL; node = node->parents[0])
    {
        if (node->num_assignments < 1) continue;
        for (int i = 0; i < node->num_assignments; i++)
        {
            assignments[abs(node->assignments[i]) - 1] = node->assignments[i] > 0;
        }
    }
}

bool dpll(formula_t *formula, bool assignments[])
{
    implication_graph_node_t root;
    implication_graph_node_init(&root, formula, 0);

    EVALUATION evaluation = bcp_init(formula, &root);
    implication_graph_node_t *leaf = &root;

    variable_map_t assignment_mirror;
    variable_map_init(&assignment_mirror, formula->num_variables);

    int *end= root.assignments + root.num_assignments;
    for (int *ass = root.assignments; ass < end; ass++) variable_map_add(&assignment_mirror, *ass, (void*) *ass);

    if (evaluation == EVALUATION_UNDETERMINED)
    {
        struct dpll_result result = _dpll(formula, &root, &assignment_mirror);
        evaluation = result.evaluation;
        leaf = result.leaf;
    }

    if (evaluation == EVALUATION_TRUE) flatten_assignments(leaf, assignments);

    implication_graph_node_delete(&root);
    return evaluation == EVALUATION_TRUE ? true : false;
}
