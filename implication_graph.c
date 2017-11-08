#include "implication_graph.h"
#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct implication_graph_node_t implication_graph_node_t;

typedef struct
{
    clause_t *clause;
    implication_graph_node_t *parent;
    implication_graph_node_t *child;
} implication_graph_edge_t;

typedef struct implication_graph_node_t
{
    int assignment;
    size_t decision_level;

    vector_t incoming_edges;
    vector_t outgoing_edges;
} implication_graph_node_t;

static size_t node_index(int assignment)
{
    return abs(assignment) - 1;
}

void implication_graph_init(implication_graph_t *implication_graph, size_t num_variables)
{
    implication_graph->nodes = malloc(num_variables * sizeof (implication_graph_node_t));

    if (implication_graph->nodes == NULL)
    {
        fprintf(stderr, "%s: Malloc failed for implication_graph->nodes. Tried to malloc %lu bytes.", __func__,
                num_variables * sizeof (implication_graph_node_t));
        exit(1);
    }

    implication_graph_node_t *end = implication_graph->nodes + num_variables;
    for (implication_graph_node_t *curr = implication_graph->nodes; curr < end; curr++)
    {
        vector_init(&curr->incoming_edges);
        vector_init(&curr->outgoing_edges);
        curr->assignment = 0;
        curr->decision_level = -1;
    }
    implication_graph->num_variables = num_variables;
    implication_graph->conflict_node = malloc(sizeof (implication_graph_node_t));
}

void implication_graph_free(implication_graph_t *implication_graph)
{
    implication_graph_node_t *end = implication_graph->nodes + implication_graph->num_variables;

    for (size_t i = 1; i < implication_graph->num_variables; i++)
    {
        implication_graph_remove_assignment(implication_graph, (int) i);
    }

    for (implication_graph_node_t *curr = implication_graph->nodes; curr < end; curr++)
    {
        vector_free(&curr->incoming_edges);
        vector_free(&curr->outgoing_edges);
    }

    free(implication_graph->nodes);
    free(implication_graph->conflict_node);
}

bool implication_graph_add_assignment(implication_graph_t *implication_graph, int assignment, size_t decision_level, int parent_assignment, clause_t *parent_clause)
{
    implication_graph_node_t *node = implication_graph-> nodes + node_index(assignment);

    node->assignment = assignment;
    node->decision_level = decision_level;

    if (parent_assignment == 0) return true;

    implication_graph_edge_t *edge = malloc(sizeof (implication_graph_edge_t));
    edge->clause = parent_clause;

    implication_graph_node_t *parent = implication_graph->nodes + node_index(parent_assignment);
    edge->parent = parent;

    if (node->assignment != 0 && node->assignment == -assignment)
    {
        edge->child = implication_graph->conflict_node;
        vector_push_back(&edge->child->incoming_edges, (void *) edge);
    }
    else
    {
        edge->child = node;
    }

    vector_push_back(&edge->parent->outgoing_edges, (void *) edge);

    return edge->child != implication_graph->conflict_node;
}

static void remove_parents(implication_graph_node_t *node)
{
    for (void **it = vector_cbegin(&node->incoming_edges); it < vector_cend(&node->incoming_edges); it++)
    {
        implication_graph_edge_t *edge = *it;
        vector_delete_ptr(&edge->parent->outgoing_edges, it);
        free(edge);
    }

    vector_free(&node->incoming_edges);
    vector_init(&node->incoming_edges);
}

static void remove_children(implication_graph_node_t *node)
{
    for (void **it = vector_cbegin(&node->outgoing_edges); it < vector_cend(&node->outgoing_edges); it++)
    {
        implication_graph_edge_t *edge = *it;
        vector_delete_ptr(&edge->child->incoming_edges, it);
        free(edge);
    }

    vector_free(&node->outgoing_edges);
    vector_init(&node->outgoing_edges);
}

void implication_graph_remove_assignment(implication_graph_t *implication_graph, int assignment)
{
    implication_graph_node_t *node = implication_graph->nodes + node_index(assignment);

    remove_parents(node);
    remove_children(node);

    node->assignment = 0;
}

int implication_graph_find_assignment(implication_graph_t *implication_graph, int variable)
{
    implication_graph_node_t *node = implication_graph->nodes + node_index(variable);
    return node->assignment;
}

void implication_graph_remove_decision_variable(implication_graph_t *implication_graph, int assignment)
{
    implication_graph_node_t *node = implication_graph->nodes + node_index(assignment);
    assert(node->assignment != 0);

    for (void **it = vector_cbegin(&node->outgoing_edges); it < vector_cend(&node->outgoing_edges); it++)
    {
        implication_graph_edge_t *edge = *it;
        implication_graph_remove_decision_variable(implication_graph, edge->child->assignment);
    }

    implication_graph_remove_assignment(implication_graph, assignment);
}


