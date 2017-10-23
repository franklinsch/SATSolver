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

void node_free(node_t *node)
{
    free(node->assignments);
    free(node->parents);
    free(node->children);
}

void node_add_parent(node_t *node, node_t *parent)
{
    node->parents[node->num_parents++] = parent;
}

void node_add_children(node_t *node, node_t *child)
{
   node->children[node->num_children++] = child;
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

/**
 From the top.
 */
static bool is_assigned(node_t *root, int variable)
{
    if (root == NULL) return false;

//    node_t **end = root->children + root->num_children;
//    for (node_t **curr = root->children; curr != end; curr++)
//    {
//        if (abs((*curr)->assignments[0]) == abs(variable)) return true;
//        if (is_assigned(*curr, variable)) return true;
//    }

    if (abs(root->assignments[0]) == abs(variable)) return true;
    if (is_assigned(root->children[0], variable)) return true;

    return false;
}

static void delete_node(node_t *curr)
{
    curr->parents[0]->children[0] = NULL;
    node_free(curr);
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
            int assignment = find_assignment(curr, variable);
            if (assignment == ASSIGNMENT_NOT_FOUND)
            {
                node_t *new_assignment = malloc(sizeof (node_t));
                node_init(new_assignment, formula, curr->depth + 1);
                new_assignment->assignments[0] = variable;
                node_add_parent(new_assignment, curr);
                node_add_children(curr, new_assignment);

                curr = new_assignment;
            }
            else if (assignment == variable)
            {
                continue;
            }
            else
            {
                node_t *parent = curr->parents[0];
                delete_node(curr);
                curr = parent;
            }
        }
    }

    // SAT

    bool all_assigned = true;

    for (int i = 1; i <= formula->num_variables; i++)
    {
        all_assigned &= is_assigned(&root, i);
    }

break_clause:
    node_free(&root);
    return all_assigned;
}
