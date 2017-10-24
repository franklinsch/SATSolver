#include "dpll.h"

#include "clause.h"
#include "formula.h"
#include "implication_graph.h"

#include <stdlib.h>
#include <stdbool.h>

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

EVALUATION _dpll(formula_t *formula, implication_graph_node_t *node)
{
    int variable = choose_var(formula->num_variables, node);
    EVALUATION evaluation = formula_evaluate(formula, node);
    
    if (evaluation == EVALUATION_UNDETERMINED)
    {
        implication_graph_node_t *child = implication_graph_node_add_child(node, variable);
        EVALUATION result = _dpll(formula, child);
        
        if (result == EVALUATION_FALSE)
        {
            implication_graph_node_delete(child);
            implication_graph_node_t *child = implication_graph_node_add_child(node, -variable);
            return _dpll(formula, child);
        }
        else
        {
            return EVALUATION_TRUE;
        }
    }
    
    return evaluation;
}

bool dpll(formula_t *formula)
{
    implication_graph_node_t root;
    implication_graph_node_init(&root, formula, 0);
    
    return _dpll(formula, &root);
}
