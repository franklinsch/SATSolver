#include "dpll.h"

#include "clause.h"
#include "formula.h"
#include "implication_graph.h"

#include <stdlib.h>
#include <stdbool.h>
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

struct dpll_result _dpll(formula_t *formula, implication_graph_node_t *node)
{
    EVALUATION evaluation = formula_evaluate(formula, node);
    
    struct dpll_result result;
    
    if (evaluation == EVALUATION_UNDETERMINED)
    {
        int variable = choose_var(formula->num_variables, node);
        
        // At least one variable should be unassigned, otherwise formula_evaluate would not
        // have returned EVALUATION_UNDETERMINED.
        assert(variable != CHOOSE_VAR_ALL_ASSIGNED);
        
        // Create a new assignment setting the variable to the positive value.
        implication_graph_node_t *child = implication_graph_node_add_child(node, variable);
        
        result = _dpll(formula, child);
        
        EVALUATION evaluation = result.evaluation;
        
        if (evaluation == EVALUATION_FALSE)
        {
            // Remove the assignment made previously.
            implication_graph_node_delete(child);
            
            // Create a new assignment with the negated value and run DPLL again.
            child = implication_graph_node_add_child(node, -variable);
            return _dpll(formula, child);
        }
        
        return result;
    }
    
    result.evaluation = evaluation;
    result.leaf = node;
    return result;
}

static void flatten_assignments(implication_graph_node_t *leaf, int assignments[])
{
    // Set all the values of assignments to DPLL_ASSIGNMENT_DONT_CARE.
    memset(assignments, DPLL_ASSIGNMENT_DONT_CARE, sizeof (int) * (leaf->formula->num_variables));
    
    for (implication_graph_node_t *node = leaf; node->depth > 0; node = node->parents[0])
    {
        assignments[abs(node->assignments[0]) - 1] = node->assignments[0];
    }
}

bool dpll(formula_t *formula, int assignments[])
{
    implication_graph_node_t root;
    implication_graph_node_init(&root, formula, 0);

    struct dpll_result result = _dpll(formula, &root);
    EVALUATION evaluation = result.evaluation;
    flatten_assignments(result.leaf, assignments);
    return evaluation == EVALUATION_TRUE ? true : false;
}
