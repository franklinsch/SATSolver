#include "bcp.h"
#include "formula.h"
#include "implication_graph.h"
#include "index_list.h"
#include "variable_map.h"

static variable_map_t g_watch_literals;
static const formula_t *g_formula;

void bcp_init(const formula_t *formula)
{
    g_formula = formula;
    variable_map_init(&g_watch_literals, formula->num_variables);
    //TODO assign the watch literals
    clause_t *end = g_formula->clauses + g_formula->num_clauses;
    for (clause_t *it = g_formula->clauses; it < end; it++)
    {
        int watch = clause_get_var(it, 0);
        variable_map
    }
}

void bcp(implication_graph_node_t *node)
{
    return;
    //TODO
}

void bcp_free()
{
    variable_map_free(&g_watch_literals);
}
