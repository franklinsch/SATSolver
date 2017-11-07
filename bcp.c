#include "bcp.h"

#include "evaluation.h"
#include "formula.h"
#include "implication_graph.h"
#include "variable_map.h"
#include "vector.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static variable_map_t g_watch_literals;
static formula_t *g_formula;

typedef struct variable_map_entry_t
{
    const void *value;
} variable_map_entry_t;

void _print_watch_literals(clause_t * clause)
{
    for (size_t i = 0; i < (g_watch_literals._num_variables * 2); i++)
    {
        variable_map_entry_t *bucket = g_watch_literals._buckets + i;
        vector_t *watched_list = (vector_t *) (bucket->value);

        if (vector_find(watched_list, (void *) clause))
        {
            int lit = i + 1 < g_watch_literals._num_variables ? ((int) i) + 1 : g_watch_literals._num_variables - i - 1;
            printf ("%d ", lit);
        }
    }
    printf("\n");
}

typedef enum
{
    // Successfully assigned a new watch literal.
    BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS,

    // No new watch literal has been set, but an assignment has been deduced.
    BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED,

    // There were no assignable variables.
    BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_FAILURE
}  BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT;

/**
 Executes one step of boolean constraint propagation for one clause

 @param node The current assignment.
 @param clause The current clause under consideration.
 @param deduction (Out) The deduction that has been made, if any.

 @return The result of this step of BCP.
 */
static BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT _bcp_assign_next_watch_literal(implication_graph_node_t *node, clause_t *clause, int *deduction)
{
    BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT res = BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_FAILURE;

    vector_t unassigned_lits;
    clause_populate_unassigned_literals(clause, node, &unassigned_lits);

    if (unassigned_lits.size == 0) goto cleanup;

    if (unassigned_lits.size == 1)
    {
        // Derive the necessary assignment in order to make the clause true.
        int unit = (int) *vector_get(&unassigned_lits, 0);
        *deduction = unit;

        res = BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED;
        goto cleanup;
    }
    // The clause has more unassigned literals.
    else
    {
        for (void **lit = vector_cbegin(&unassigned_lits); lit < vector_cend(&unassigned_lits); lit++)
        {
            int assignment = *(int *)lit;
            vector_t *watched = variable_map_get(&g_watch_literals, assignment);
            if (!vector_find(watched, clause))
            {
                // We have found an unassigned literal that is not currently watching this clause.
                // Add this clause to the literals watch list.
                vector_push_back(watched, (void *) clause);
                res = BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS;
                goto cleanup;
            }
        }

        // If we try to find another watch literal, we should have at least one unassigned available.
//        assert(false);
    }

cleanup:
    vector_free(&unassigned_lits);
    return res;
}

static void variable_map_init_clauses(variable_map_t *map, size_t num_variables)
{
    for (int sign = -1; sign <= 1; sign += 2) {
        for (int i = 1; i <= num_variables; i++)
        {
            vector_t *vector = malloc(sizeof (vector_t));
            vector_init(vector);
            variable_map_add(map, sign * i, vector);
        }
    }
}

static EVALUATION clause_evaluate_with_node(clause_t *clause, implication_graph_node_t *node)
{
    EVALUATION evaluation = EVALUATION_FALSE;
    for (void **it = vector_cbegin(&clause->variables); it < vector_cend(&clause->variables); it++)
    {
        int it_val = (int) *it;
        int assignment_value = implication_graph_find_assignment(node, it_val);

        if (assignment_value == it_val) return EVALUATION_TRUE;
        if (assignment_value == ASSIGNMENT_NOT_FOUND) evaluation = EVALUATION_UNDETERMINED;
    }
    return evaluation;
}

EVALUATION bcp_init(const formula_t *formula, implication_graph_node_t *root)
{
    EVALUATION evaluation = EVALUATION_TRUE;

    g_formula = formula;
    variable_map_init(&g_watch_literals, formula->num_variables);
    variable_map_init_clauses(&g_watch_literals, formula->num_variables);

    clause_t *end = g_formula->clauses + g_formula->num_clauses;
    size_t num_deductions;

    vector_t non_trivial_clauses;
    vector_init(&non_trivial_clauses);
    for (clause_t *clause = g_formula->clauses; clause < end; clause++)
    {
        vector_push_back(&non_trivial_clauses, (void *) clause);
    }

    size_t *num_watch_literals = calloc(formula->num_clauses, sizeof (size_t));

    // Keep weeding out trivial clauses as long as we have made deductions in the previous round.
    do
    {
        num_deductions = 0;
        for (clause_t *clause = g_formula->clauses; clause < end; clause++)
        {
            int deduction = 0;

            if (num_watch_literals[clause - formula->clauses] >= 2) continue;

            BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT assignment_result
            = _bcp_assign_next_watch_literal(root, clause, &deduction);

            if (assignment_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED)
            {
                if (implication_graph_find_assignment(root, deduction) == -deduction)
                {
                    return EVALUATION_FALSE;
                }

                if (clause_evaluate_with_node(clause, root) != EVALUATION_TRUE)
                {
#ifdef DEBUG
                    fprintf(stderr, "Deduction: %d from ", deduction);
#endif
                    clause_print(clause);
                    implication_graph_node_add_assignment(root, deduction);
                    num_deductions++;
                }
                else
                {
                    void **elem = vector_find(&non_trivial_clauses, (void *) clause);
                    if (elem)
                    {
                        vector_delete(&non_trivial_clauses, elem - vector_cbegin(&non_trivial_clauses));
                    }
                }
            }
            else if (assignment_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_FAILURE)
            {
                EVALUATION clause_evaluation = clause_evaluate_with_node(clause, root);
                if (clause_evaluation == EVALUATION_FALSE) return EVALUATION_FALSE;
                if (clause_evaluation == EVALUATION_TRUE)
                {
                    void **elem = vector_find(&non_trivial_clauses, (void *) clause);
                    if (elem)
                    {
                        vector_delete(&non_trivial_clauses, elem - vector_cbegin(&non_trivial_clauses));
                    }
                }
            }
            else
            {
                // This should never fail as the clause is already undetermined, and thus has 2 unassigned watch literals.
                _bcp_assign_next_watch_literal(root, clause, &deduction);
                num_watch_literals[clause - formula->clauses] += 2;
                evaluation = EVALUATION_UNDETERMINED;
            }

        }
    } while (num_deductions > 0);

    int num_variables = g_formula->num_variables;
//    formula_free(g_formula);
    formula_init(g_formula, non_trivial_clauses.size, num_variables);
    for (void **it = vector_cbegin(&non_trivial_clauses); it < vector_cend(&non_trivial_clauses); it++)
    {
        formula_add_clause(g_formula, *(clause_t*) *it);
    }

    return evaluation;
}

void bcp(implication_graph_node_t *node, variable_map_t *assignment_mirror)
{
    vector_t pending_assignments;
    vector_init(&pending_assignments);

    int *end = node->assignments + node->num_assignments;
    for (int *curr = node->assignments; curr < end; curr++)
    {
        vector_push_back(&pending_assignments, (void *) curr);
    }

    // Process all the necessary assignments.
    while (pending_assignments.size > 0)
    {
        // TODO: substitute with dequeue
        int assignment = (int) *vector_get(&pending_assignments, 0);
        vector_delete(&pending_assignments, 0);

        // We find the clauses associated with the watch literals of opposite polarity.
        vector_t *watch_list = variable_map_get(&g_watch_literals, -assignment);

        for (void **cl = vector_cbegin(watch_list); cl < vector_cend(watch_list); cl++)
        {
            clause_t *clause = (clause_t *) *cl;
            int deduction = 0;

            if (clause_evaluate(clause, assignment_mirror, NULL) == EVALUATION_TRUE)
            {
                continue;
            }

            BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT iteration_result
            = _bcp_assign_next_watch_literal(node, clause, &deduction);

            if (iteration_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED)
            {
                implication_graph_node_add_assignment(node, deduction);
                variable_map_add(assignment_mirror, deduction, (void *) deduction);

                // Find any potential assignments, at the same depth.
                vector_push_back(&pending_assignments, (void *) deduction);
            }
            else if (iteration_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS)
            {
                // Remove the previous watch literal
                if (watch_list->size == 1) { printf(""); }
                vector_delete(watch_list, cl - vector_cbegin(watch_list));

                // Update the clause pointer.
                cl--;
            }
        }
    }

cleanup:
    vector_free(&pending_assignments);
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
