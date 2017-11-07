#include "implication_graph.h"
#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void implication_graph_node_init(implication_graph_node_t *node, formula_t *formula, unsigned depth)
{
    node->depth = depth;
    node->formula = formula;

    node->assignments = calloc(formula->num_variables, sizeof (int));
    node->num_assignments = 0;

    node->parents = calloc(1, sizeof (implication_graph_node_t *));
    node->num_parents = 0;

    node->children = calloc(1, sizeof (implication_graph_node_t *));
    node->num_children = 0;
}

static void _implication_graph_node_free(implication_graph_node_t *node)
{
    free(node->assignments);
    free(node->parents);
    free(node->children);
}

static void implication_graph_node_add_parent(implication_graph_node_t *node, implication_graph_node_t *parent)
{
    node->parents[node->num_parents++] = parent;
}

void implication_graph_node_add_assignment(implication_graph_node_t *node, int variable)
{
    node->assignments[node->num_assignments++] = variable;
}

implication_graph_node_t *implication_graph_node_add_child(implication_graph_node_t *node, int variable)
{
    if (abs(variable) > node->formula->num_variables)
    {
        fprintf(stderr, "%s: Variable %d is invalid for the current formula.", __func__, variable);
        return NULL;
    }

    implication_graph_node_t *child = malloc(sizeof (implication_graph_node_t));
    implication_graph_node_init(child, node->formula, node->depth + 1);
    child->assignments[child->num_assignments++] = variable;

    node->children[node->num_children++] = child;
    implication_graph_node_add_parent(child, node);
    
    return child;
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
                par->num_children--;
            }
        }
    }

    implication_graph_node_t *child_end = *(node->children) + node->num_children;
    for (implication_graph_node_t *child = *(node->children); child < child_end; child++)
    {
        implication_graph_node_delete(child);
    }

    // We have cleaned up all the references to ourselves so we can clean up.
    _implication_graph_node_free(node);
    // Non root nodes are allocated by us, so we can free them, however we don't know where root lives so we leave this
    // responsibility to its' owner.
    if (node->depth > 0)
    {
        free(node);
    }
}

int implication_graph_find_assignment(implication_graph_node_t *node, int variable)
{
    int *ass_end = node->assignments + node->num_assignments;
    for (int *ass = node->assignments; ass < ass_end; ass++)
    {
        if (abs(*ass) == abs(variable))
            return *ass;
    }

    implication_graph_node_t *par_end = *(node->parents) + node->num_parents;

    for (implication_graph_node_t *par = *(node->parents); par < par_end; par++)
    {
        int result = implication_graph_find_assignment(par, variable);
        if (result != ASSIGNMENT_NOT_FOUND) return result;
    }

    return ASSIGNMENT_NOT_FOUND;
}

