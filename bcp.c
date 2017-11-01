#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "index_list.h"
#include "variable_map.h"

static variable_map_t g_watch_literals;
static const formula_t *g_formula;

EVALUATION bcp_init(const formula_t *formula, implication_graph_node_t *root)
{
    g_formula = formula;
    variable_map_init(&g_watch_literals, formula->num_variables);
    clause_t *end = g_formula->clauses + g_formula->num_clauses;
    for (clause_t *it = g_formula->clauses; it < end; it++)
    {
        size_t clause_index = it - g_formula->clauses;
        if (it->size > 1)
        {
            // Get to watch literals we probably need a better heuristic for choosing them.
            for (int i = 0; i < 2; i++)
            {
                int watch = clause_get_var(it, i);
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
            
        }
    }
    return EVALUATION_UNDETERMINED;
}

void bcp(implication_graph_node_t *node)
{
    
}

void bcp_free(void)
{
    variable_map_free(&g_watch_literals);
}
