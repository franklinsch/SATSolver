#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "variable_map.h"
#include "vector.h"

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

    vector_t unassigned_lits;
    clause_get_unassigned_literals(clause, root, &unassigned_lits);

    if (unassigned_lits.size > 1)
    {
        // This clause's satisfiability is not trivially decidable since it contains more than one unassigned literal.
        evaluation = EVALUATION_UNDETERMINED;

        for (int i = 0; i < 2; i++)
        {
            int watch = (int) *vector_get(&unassigned_lits, i);
            vector_t *watch_list = variable_map_get(&g_watch_literals, watch);

            if (watch_list == NULL)
            {
                watch_list = malloc(sizeof (vector_t));
                vector_init(watch_list);
                variable_map_add(&g_watch_literals, watch, watch_list);
            }

            vector_push_back(watch_list, (void *) clause);
        }

    }
    else
    {
        int assignment = clause_get_var(clause, 0);

        int existing_assignment = implication_graph_find_assignment(root, assignment);
        if (existing_assignment == -assignment) return EVALUATION_FALSE;

        implication_graph_node_add_assignment(root, assignment);
    }

    vector_free(&unassigned_lits);
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
    vector_t pending_assignments;
    vector_init(&pending_assignments);

    // the first assignment should be made by DPLL, any following ones are done via unit resolution
    assert(node->num_assignments == 0);
    vector_push_back(&pending_assignments, (void *) node->assignments[0]);

    // We only need to worry about the negative assignments for each literals
    // Therefore we only update the clauses with watch literal -assignment
    while (pending_assignments.size > 0)
    {
        // TODO: substitute with dequeue
        int assignment = (int) *vector_get(&pending_assignments, 0);
        vector_delete(&pending_assignments, 0);

        vector_t *clauses = variable_map_get(&g_watch_literals, assignment);

        for (void **cl = vector_cbegin(clauses); cl < vector_cend(clauses); cl++)
        {
            clause_t *clause = (clause_t *) *cl;
            vector_t unassigned_lits;
            clause_get_unassigned_literals(clause, node, &unassigned_lits);

            if (unassigned_lits.size == 1)
            {
                // Assign unit literal left in clause to true
                int unit = (int) *vector_get(&unassigned_lits, 0);
                implication_graph_node_add_assignment(node, unit);

                // Recurse on bcp to resolve potentian new unit literals
                vector_push_back(&pending_assignments, (void *) unit);

            }
            // The clause has more unassigned literals
            else
            {
                for (void **lit = vector_cbegin(&unassigned_lits); lit < vector_cend(&unassigned_lits); lit++)
                {
                    vector_t *watched = variable_map_get(&g_watch_literals, (int) *lit);
                    // We have found an unassigned literal that is not currently watching this clause
                    if (!vector_find(watched, cl))
                    {
                        // Add this clause to the literals watch list
                        vector_push_back(watched, (void *) clause);
                        break;
                    }
                }
            }
        }
    }
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
