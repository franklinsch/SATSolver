#include "implication_graph.h"
#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void implication_graph_node_init(implication_graph_node_t *node, formula_t *formula, unsigned depth)
{
    node->depth = depth;

    node->assignments = calloc(formula->num_variables, sizeof (int));
    node->num_assignments = formula->num_variables;

    node->parents = calloc(1, sizeof (implication_graph_node_t *));
    node->num_parents = 0;

    node->children = calloc(1, sizeof (implication_graph_node_t *));
    node->num_children = 0;
}

void implication_graph_node_free(implication_graph_node_t *node)
{
    //TODO: Fix this
    free(node->assignments);
    free(node->parents);
    free(node->children);
}

static void implication_graph_node_add_parent(implication_graph_node_t *node, implication_graph_node_t *parent)
{
    node->parents[node->num_parents++] = parent;
}

void implication_graph_node_add_child(implication_graph_node_t *node, int variable)
{
    if (abs(variable) > node->formula->num_variables)
    {
        fprintf(stderr, "%s: variable %d is invalid for the current formula.", __func__, variable);
        return;
    }

    implication_graph_node_t *child = malloc(sizeof (implication_graph_node_t));
    implication_graph_node_init(child, node->formula, node->depth + 1);
    child->assignments[child->num_assignments++] = variable;

    node->children[node->num_children++] = child;
    implication_graph_node_add_parent(child, node);
}

void implication_graph_node_delete(implication_graph_node_t *node)
{
    implication_graph_node_t *par_end = *(node->parents) + node->num_parents;
    for (implication_graph_node_t *par = *(node->parents); par < par_end; par++)
    {
        implication_graph_node_t *child_end = *(par->children) + par->num_children;
        for (implication_graph_node_t *child = *(par->children); child < child_end; child++)
        {
            if (child == node)
            {
                // Delete the reference to ourselves, we need the children array to be contiguous memory in the parent,
                // so we move the next children's references on top of our reference.
                void *dst = child++;
                const void *src = child;
                // We are moving pointers so pointer difference is correct.
                size_t n = child_end - child;
                memmove(dst, src, n);
            }
        }
    }

    // We have cleaned up all the references to ourselves so we can clean up.
    implication_graph_node_free(node);
    free(node);
}

int implication_graph_find_assignment(implication_graph_node_t *node, int variable)
{
    int *ass_end = node->assignments + node->num_assignments;
    for (int *ass = node->assignments; ass < ass_end; ass++)
    {
        if (abs(*ass) == abs(variable))
            return *ass;
    }

    int *par_end = *(node->parents) + node->num_parents;
    int assigned = ASSIGNMENT_NOT_FOUND;
    for (implication_graph_node_t *par = *(node->parents); par < par_end; par++)
    {
        assigned |= implication_graph_find_assignment(par, variable);
    }

    return assigned;
}

