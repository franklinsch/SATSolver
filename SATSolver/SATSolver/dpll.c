#include "dpll.h"
#include "formula.h"
#include "clause.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t
{
    int depth;
    
    int *assignments;
    size_t num_assignments;
    
    struct node_t **parents;
    size_t num_parents;
    
    struct node_t **children;
    size_t num_children;
} node_t;

void node_init(node_t *node, formula_t *formula, int depth)
{
    node->depth = depth;
    
    node->assignments = malloc(sizeof (int) * formula->num_variables);
    node->assignments[0] = 0;
    node->num_assignments = formula->num_variables;
    
    node->parents = malloc(sizeof (node_t *));
    node->parents[0] = NULL;
    node->num_parents = 0;
    
    node->children = malloc(sizeof (node_t *));
    node->num_children = 0;
}

#define ASSIGNMENT_NOT_FOUND 0

/**
 Traverses the assignment tree upwards to try and find an assignment for the given variable.
 
 @return The weighted variable.
 */
static int find_assignment(node_t *node, int variable)
{
    for (node_t *curr = node; curr && curr->depth >= 0; curr = curr->parents[0])
    {
        if (abs(curr->assignments[0]) == abs(variable))
            return curr->assignments[0];
    }
    
    return ASSIGNMENT_NOT_FOUND;
}

bool dpll(formula_t *formula)
{
    
    node_t root;
    node_init(&root, formula, 0);
    
    node_t *curr = &root;
    
    clause_t *end = formula->clauses + formula->num_clauses;
    for (clause_t *clause = formula->clauses; clause != end; clause++)
    {
        for (int i = 0; i < clause->size; i++)
        {
            int variable = clause_get_var(clause, i);
            if (find_assignment(curr, variable) == ASSIGNMENT_NOT_FOUND)
            {
                node_t new_assignment;
                node_init(&new_assignment, formula, 0);
                new_assignment.parents[0] = curr;
                curr->children[0] = &new_assignment;
            }
        }
    }
    
    free(root.children);
    free(root.parents);
    free(root.assignments);
    return false;
}
