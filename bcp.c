#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "index_list.h"
#include "variable_map.h"

static variable_map_t g_watch_literals;
static const formula_t *g_formula;

static EVALUATION _bcp_clause_assign_watch_literals(clause_t *clause, implication_graph_node_t *root) {
    EVALUATION evaluation = EVALUATION_TRUE;

    size_t clause_index = clause - g_formula->clauses;
    if (clause->size > 1)
    {
        // Get to watch literals we probably need a better heuristic for choosing them.
        for (int i = 0; i < 2; i++)
        {
            evaluation = EVALUATION_UNDETERMINED;
            int watch = clause_get_var(clause, i);
            index_list_t *watch_list = variable_map_get(&g_watch_literals, watch);
            if (watch_list == NULL)
            {
                watch_list = malloc(sizeof (index_list_t));
                index_list_init(watch_list);
                variable_map_add(&g_watch_literals, watch, watch_list);
            }
            // Store an index into the formula clauses to represent the watched clause.
            index_list_append(watch_list, clause_index);
        }
    }
    else
    {
        int assignment = clause_get_var(clause, 0);

        int existing_assignment = implication_graph_find_assignment(root, assignment);
        if (existing_assignment == -assignment) return EVALUATION_FALSE;

        implication_graph_node_add_assignment(root, assignment);
    }

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
    
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
