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

    // the first assignment should be made by DPLL, any following ones are done via unit resolution
    assert(node->num_assignments == 0);
    list_append(&pending_assignments, (void *) node->assignments[0]);

    // We only need to worry about the negative assignments for each literals
    // Therefore we only update the clauses with watch literal -assignment
    if (pending_assignments.size > 0)
    {
        // TODO: substitute with dequeue
        int assignment = list_get_at(&pending_assignments, 0);
        list_remove_at(&pending_assignments, 0);

        list_t *clauses = variable_map_get(&g_watch_literals, assignment);
        list_iterator_t *clauses_it = list_get_iterator(clauses);

        for (clause_t *cl = list_iterator_get(clauses_it); list_iterator_has_next(clauses_it); cl = list_iterator_next(clauses_it))
        {
            variable_vector_t unassigned_lits;
            variable_vector_init(&unassigned_lits);
            clause_get_unassigned_literals(cl, node, &unassigned_lits);

            if (unassigned_lits.size == 1)
            {
                // Assign unit literal left in clause to true
                int unit = *variable_vector_get(&unassigned_lits, 0);
                implication_graph_node_add_assignment(node, unit);

                // Recurse on bcp to resolve potentian new unit literals
                list_append(&pending_assignments, (void *) unit);

            }
            // The clause has more unassigned literals
            else
            {
                for (int *lit = variable_vector_cbegin(&unassigned_lits);
                        lit < variable_vector_cend(&unassigned_lits);
                        lit++)
                {
                    list_t *watched = variable_map_get(&g_watch_literals, *lit);
                    // We have found an unassigned literal that is not currently watching this clause
                    if (!list_find(watched, cl))
                    {
                        // Add this clause to the literals watch list
                        list_append(watched, cl);
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
