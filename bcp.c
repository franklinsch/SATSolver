#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "variable_map.h"
#include "vector.h"

#include <assert.h>

static variable_map_t g_watch_literals;
static const formula_t *g_formula;

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

    if (unassigned_lits.size == 0) return BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_FAILURE;

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
            vector_t *watched = variable_map_get(&g_watch_literals, (int) *lit);
            if (!vector_find(watched, clause))
            {
                // We have found an unassigned literal that is not currently watching this clause.
                // Add this clause to the literals watch list.
                vector_push_back(watched, (void *) clause);
                res = BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS;
                goto cleanup;
            }
        }

        // If we try to find another watch literal we should have at least one unassigned available
        assert(0);
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

EVALUATION bcp_init(const formula_t *formula, implication_graph_node_t *root)
{
    EVALUATION evaluation = EVALUATION_TRUE;

    g_formula = formula;
    variable_map_init(&g_watch_literals, formula->num_variables);
    variable_map_init_clauses(&g_watch_literals, formula->num_variables);

    clause_t *end = g_formula->clauses + g_formula->num_clauses;

    for (clause_t *clause = g_formula->clauses; clause < end; clause++)
    {
        int deduction;
        BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT assignment_result
            = _bcp_assign_next_watch_literal(root, clause, &deduction);
        if (assignment_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED)
        {
            if (implication_graph_find_assignment(root, deduction) == -deduction)
            {
                return EVALUATION_FALSE;
            }

            implication_graph_node_add_assignment(root, deduction);
        }
        else if (assignment_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_FAILURE)
        {
            return EVALUATION_FALSE;
        }
        else
        {
            // assignment_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS
            assignment_result = _bcp_assign_next_watch_literal(root, clause, &deduction);
            evaluation = EVALUATION_UNDETERMINED;
        }
    }

    return evaluation;
}

EVALUATION bcp(implication_graph_node_t *node)
{
    vector_t pending_assignments;
    vector_init(&pending_assignments);

    // The first assignment should be made by DPLL, any following ones are done via unit resolution.
//    assert(node->num_assignments == 0);

    // If DPLL has not made any assignments, no deductions can be made.
    if (node->num_assignments < 1) return EVALUATION_UNDETERMINED;

    vector_push_back(&pending_assignments, (void *) node->assignments[0]);

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
            BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT iteration_result
                = _bcp_assign_next_watch_literal(node, clause, &deduction);

            if (iteration_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_DEDUCED)
            {
                implication_graph_node_add_assignment(node, deduction);

                // Find any potential assignments, at the same depth.
                vector_push_back(&pending_assignments, (void *) deduction);
            }
            else if (iteration_result == BCP_ASSIGN_NEXT_WATCH_LITERAL_RESULT_SUCCESS)
            {
                // Remove the previous watch literal
                vector_delete(watch_list, cl - vector_cbegin(watch_list));
            }
            else
            {
                vector_t unassigned_lits;
                vector_init(&unassigned_lits);

                EVALUATION evaluation = clause_evaluate(clause, node, &unassigned_lits);
                assert(evaluation != EVALUATION_UNDETERMINED);
                return evaluation;
            }
        }
    }

    return EVALUATION_UNDETERMINED;
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
