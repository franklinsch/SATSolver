#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "list.h"
#include "variable_map.h"
#include "variable_vector.h"

#include <assert.h>

static variable_map_t g_watch_literals;
static const formula_t *g_formula;

/**
 Assign the watch literals for the given clause. Deduces trivial assignments if the clause has only one
 literal.

 @param root The root of the implication graph.
 @return Whether the clause's satisfiability is trivially decidable.
 */
static EVALUATION _bcp_clause_assign_watch_literals(clause_t *clause, implication_graph_node_t *root) {
    EVALUATION evaluation = EVALUATION_TRUE;

    variable_vector_t unassigned_lits;
    variable_vector_init(&unassigned_lits);
    clause_get_unassigned_literals(clause, root, &unassigned_lits);

    if (unassigned_lits.size > 1)
    {
        // This clause's satisfiability is not trivially decidable since it contains more than one unassigned literal.
        evaluation = EVALUATION_UNDETERMINED;

        for (int i = 0; i < 2; i++)
        {
            int watch = *variable_vector_get(&unassigned_lits, i);
            list_t *watch_list = variable_map_get(&g_watch_literals, watch);

            if (watch_list == NULL)
            {
                watch_list = malloc(sizeof (list_t));
                list_init(watch_list);
                variable_map_add(&g_watch_literals, watch, watch_list);
            }

            list_append(watch_list, (void *) clause);
        }

    }
    else
    {
        int assignment = clause_get_var(clause, 0);

        int existing_assignment = implication_graph_find_assignment(root, assignment);
        if (existing_assignment == -assignment) return EVALUATION_FALSE;

        implication_graph_node_add_assignment(root, assignment);
    }

    variable_vector_free(&unassigned_lits);
    return evaluation;
}

EVALUATION bcp_init(const formula_t *formula, implication_graph_node_t *root)
{
    EVALUATION evaluation = EVALUATION_TRUE;

    g_formula = formula;
    variable_map_init(&g_watch_literals, formula->num_variables);
    clause_t *end = g_formula->clauses + g_formula->num_clauses;
    for (clause_t *clause = g_formula->clauses; clause < end; clause++)
    {
        EVALUATION clause_evaluation = _bcp_clause_assign_watch_literals(clause, root);
        if (clause_evaluation == EVALUATION_FALSE) return EVALUATION_FALSE;
        if (clause_evaluation == EVALUATION_UNDETERMINED) evaluation = EVALUATION_UNDETERMINED;
    }

    return evaluation;
}

void bcp(implication_graph_node_t *node)
{
    list_t pending_assignments;
    list_init(&pending_assignments);

    // The node should only have one assignment (added by DPLL).
    assert(node->num_assignments == 1);

    list_t *clauses = variable_map_get(&g_watch_literals, node->assignments[0]);
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
