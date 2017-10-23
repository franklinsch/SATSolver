//
//  cdcl.c
//  SATSolver
//
//  Created by Franklin Schrans on 10/22/17.
//  Copyright © 2017 Franklin Schrans. All rights reserved.
//

#include "cdcl.h"
#include "formula.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t {
  int *values;
  int num_values;
  
  struct node_t *parents;
  int num_parents;
  
  struct node_t *children;
  int num_children;
} node_t;

#define NODE_DEFAULT_MAX_VALUES 8

void node_init(node_t *node)
{
  node->values = malloc(sizeof(int) * NODE_DEFAULT_MAX_VALUES);
}

void node_add_value(node_t *node, int value)
{
  node->values[node->num_values++] = value;
}

int find_assignment(int variable, node_t *node)
{
  if (node == NULL) return 0;
  
  for (int i = 0; i < node->num_values; i++) {
    int value = node->values[i];
    if (variable == abs(value)) {
      return value;
    }
  }
  
  if (node->num_children != 0) {
    for (int i = 0; i < node->num_children; i++) {
      int assignment = find_assignment(variable, &node->children[i]);
      if (assignment != 0) return assignment;
    }
  }
  
  return 0;
}

int count_unassigned_variables(clause_t *clause, node_t *root)
{
  int count = 0;
  for (int i = 0; i < clause->size; i++) {
    int variable = clause->variables[i];
    if (find_assignment(variable, root) == 0) count++;
  }
  
  return count;
}


bool cdcl(formula_t *formula)
{
  node_t *root = NULL;
  
  // Find trivial assignments.
  for (int i = 0; i < formula->num_clauses; i++) {
    clause_t *clause = &formula->clauses[i];
    if (clause->size == 1) {
      int value = clause->variables[0];
      root = malloc(sizeof(node_t));
      node_init(root);
      node_add_value(root, value);
    }
  }
  
  if (root == NULL) {
    // No trivial assignments were found. Pick one.
  }
  
  for (int i = 0; i < formula->num_clauses; i++) {
    clause_t *clause = &formula->clauses[i];
    int num_unassigned_variables = count_unassigned_variables(clause, root);
    if (num_unassigned_variables == 1) {
      
    }
  }
  
  return false;
}
