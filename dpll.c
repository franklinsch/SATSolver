#include "dpll.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "clause.h"
#include "list.h"
#include "assignment.h"

// -- Helper data structures.

typedef struct {
  clause_t *clause;
  
  // The indices of the watched literals in the clause (2 maximum).
  int watch_literals_indices[2];
} bcp_clause_t;

// -- Forward function declarations

static formula_t *bcp(formula_t *formula);
static bool _dpll(formula_t *formula, int depth);

// -- Implementations

bool dpll(formula_t *formula)
{
  return _dpll(formula, 0);
}

static bool _dpll(formula_t *formula, int depth)
{
  assert(formula->num_variables > 0);
  assignment_t **assignments = malloc(sizeof(formula->num_variables) * sizeof(assignment_t *));
  // list_t assignment_queue = list_alloc();
  
  clause_t *end = formula->clauses + formula->num_clauses;
  for (clause_t *c = formula->clauses; c < end; ++c)
  {
    if (c->size == 1)
    {
      assignment_t *assignment = malloc(sizeof (assignment));
      int variable = *(c->variables);
      assignment->variable = abs(variable);
      assignment->value = variable < 0 ? false : true;
      assignment->depth = depth;
      
      if (assignments[abs(variable)] != NULL &&
          assignments[abs(variable)]->value != assignment->value)
      {
        // Violation
        fprintf(stderr, "Variable %d has incompatible assignments.\n", abs(variable));
        return false;
      }
      
      assignments[variable] = assignment;
    }
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
