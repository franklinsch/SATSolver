#include "dpll.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "clause.h"
#include "list.h"

// -- Helper data structures.

typedef struct {
  clause_t *clause;
  
  // The indices of the watched literals in the clause (2 maximum).
  int watch_literals_indices[2];
} bcp_clause_t;

// Represents an assignment to be made to a literal.
typedef struct {
  int literal;
  
  // The target value of literal after the assignment.
  bool assignment;
} assignment_t;

// -- Forward function declarations

static formula_t *bcp(formula_t *formula);
static list_t *get_trivial_assignments(formula_t *formula);

// -- Implementations

bool dpll(formula_t *formula) 
{
  assert(formula->num_variables > 0);
  bool assignments[formula->num_variables];
  // list_t assignment_queue = list_alloc();
  
  // Get all trivial assignments.

  list_t *trivial_assignments = get_trivial_assignments(formula);
  for (size_t i = 0; i < trivial_assignments->_size; ++i)
  {
    int assignment = list_get_at(trivial_assignments, i)->value;
    if (assignments[abs(assignment)] != assignment)
    {
      // Two trivial clauses have conflicting variable values.
      return false;
    }
    assignments[abs(assignment)] = assignment < 0 ? -1 : 1;
  }

  // hashmap_t *watch_literal_map = hashmap_alloc();
  
  return false;
}

/*
 * Applies Boolean Constant Propagation to `formula`, returning the resulting formula.
 */
static formula_t *bcp(formula_t *formula)
{
  return NULL;
}

/*
 * Returns the list of assignments implied by size one clauses.
 */
static list_t *get_trivial_assignments(formula_t *formula)
{
  list_t *assignments = list_new();

  clause_t *end = formula->clauses + formula->num_clauses;
  for (clause_t *c = formula->clauses; c < end; ++c) 
  {
    if (c->size == 1) 
    {
      list_append(assignments, *(c->variables));
    }
  }

  return assignments;
}
