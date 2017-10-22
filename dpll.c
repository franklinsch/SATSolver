#include "dpll.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "clause.h"
#include "list.h"
#include "assignment.h"

enum BCP_WATCH
{
  BCP_UNWATCHED,
  BCP_WATCHING,
  BCP_WATCHED
};

enum BCP_WATCH_ASSIGNMENT
{
  // Successfully set two literals as watched.
  BCP_WATCH_ASSIGNMENT_SUCCESS,
  
  // All the literals have been watched.
  BCP_WATCH_ASSIGNMENT_WATCHED_ALL,
  
  // There is only one variable in the clause.
  BCP_WATCH_ASSIGNMENT_SIZE_ONE
};

// -- Helper data structures.

typedef struct
{
  clause_t *clause;
  size_t size;
  
  // The indices of the watched literals in the clause (2 maximum).
  int *watch_map;
} bcp_clause_t;

typedef struct
{
  assignment_t **assignments;
  int size;
} assignment_queue_t;

// -- Forward function declarations

static bool _dpll(formula_t *formula, int depth);
static void bcp_clause_init(bcp_clause_t *bcp_clause, clause_t *clause);
static int assign_watch_literals(bcp_clause_t *bcp_clause, assignment_queue_t *assignment_queue, int depth);
static size_t assignment_queue_add(assignment_queue_t *queue, assignment_t *assignment);
static void assignment_queue_init(assignment_queue_t *assignment_queue, size_t size);
static bool are_assignments_valid(assignment_queue_t *assignment_queue, int num_variables);

// -- Implementations

bool dpll(formula_t *formula)
{
  return _dpll(formula, 0);
}

static bool _dpll(formula_t *formula, int depth)
{
  // Assign watch literals
  
  bool **watch_literals = malloc(sizeof(bool *) * formula->num_clauses);
  
  assignment_queue_t *queue = malloc(sizeof(assignment_queue_t));
  assignment_queue_init(queue, 512);
  
  for (int i = 0; i < formula->num_clauses; ++i)
  {
    clause_t *clause = &formula->clauses[i];
    
    watch_literals[i] = malloc(sizeof(int) * clause->size);
    
    int num_watching = 0;
    int assignable = 0;
    for (int j = 0; j < clause->size && num_watching < 2; ++j)
    {
      if (!watch_literals[i][j])
      {
        watch_literals[i][j] = true;
        assignable = j;
        num_watching++;
      }
    }
    
    if (num_watching != 2)
    {
      int value = clause->variables[assignable];
      assignment_t *assignment = malloc(sizeof(assignment_t));
      assignment->value = value;
      assignment->depth = depth;
      assignment_queue_add(queue, assignment);
      continue;
    }
  }
  
  if (!are_assignments_valid(queue, formula->num_variables))
  {
    if (depth < 1) return false;
  }
  
  return false;
  
  // If not possible, deduce assignments
  // If incompatible, backtrack
}

static bool are_assignments_valid(assignment_queue_t *assignment_queue, int num_variables)
{
  // TODO: Make this faster.
  
  for (int i = 0; i < assignment_queue->size; i++)
  {
    for (int j = 0; j < assignment_queue->size; j++)
    {
      if (i == j) continue;
      int value1 = assignment_queue->assignments[i]->value;
      int value2 = assignment_queue->assignments[j]->value;
      
      if (abs(value1) == abs(value2) && value1 != value2)
      {
        fprintf(stderr, "Variable %d has been assigned different truth values.\n", abs(value1));
        return false;
      }
    }
  }
  
  return true;
}

//static bool _dpll(formula_t *formula, int depth)
//{
//  assert(formula->num_variables > 0);
//
//  assignment_t **assignments = malloc(sizeof(formula->num_variables) * sizeof(assignment_t *));
//
//  assignment_queue_t *assignment_queue = malloc(sizeof(assignment_queue_t));
//  assignment_queue_init(assignment_queue, 512);
//
//  clause_t *end = formula->clauses + formula->num_clauses;
//  for (clause_t *c = formula->clauses; c < end; ++c)
//  {
////    if (c->size == 1)
////    {
////      assignment_t *assignment = malloc(sizeof (assignment));
////      int variable = *(c->variables);
////      assignment->variable = abs(variable);
////      assignment->value = variable < 0 ? false : true;
////      assignment->depth = depth;
////
////      if (assignments[abs(variable)] != NULL &&
////          assignments[abs(variable)]->value != assignment->value)
////      {
////        // Violation
////        fprintf(stderr, "Variable %d has incompatible assignments.\n", abs(variable));
////        return false;
////      }
////
////      assignments[variable] = assignment;
////    }
//
//    bcp_clause_t *bcp_clause = malloc(sizeof(bcp_clause));
//    bcp_clause_init(bcp_clause, c);
//
//    int res = assign_watch_literals(bcp_clause, assignment_queue, depth);
//
//    int count = 0;
//    for (int i = 0; i < bcp_clause->size; i++)
//    {
//      if (bcp_clause->watch_map[i] == BCP_WATCHING)
//      {
//        count++;
//      }
//    }
//
//    printf("Count is %d (res=%d)\n", count, res);
//  }
//
//  // hashmap_t *watch_literal_map = hashmap_alloc();
//
//  return false;
//}

static void bcp_clause_init(bcp_clause_t *bcp_clause, clause_t *clause)
{
  size_t num_variables = clause->size;
  bcp_clause->size = num_variables;
  
  bcp_clause->clause = clause;
  
  bcp_clause->watch_map = malloc(sizeof(int) * num_variables);
  
  for (int i = 0; i < num_variables; i++)
  {
    bcp_clause->watch_map[i] = BCP_UNWATCHED;
  }
}

static int assign_watch_literals(bcp_clause_t *bcp_clause, assignment_queue_t *assignment_queue, int depth)
{
  if (bcp_clause->size == 1)
  {
    assignment_t *assignment = malloc(sizeof(assignment_t));
    assignment->value = bcp_clause->clause->variables[0];
    assignment->depth = depth;
    assignment_queue_add(assignment_queue, assignment);
    return BCP_WATCH_ASSIGNMENT_SIZE_ONE;
  }
  
  int num_assigned = 0;
  
  for (int i = 0; i < bcp_clause->size && num_assigned < 3; i++)
  {
    if (bcp_clause->watch_map[i] == BCP_UNWATCHED)
    {
      bcp_clause->watch_map[i] = BCP_WATCHING;
    }
  }
  
  if (num_assigned == 2)
  {
    return BCP_WATCH_ASSIGNMENT_SUCCESS;
  }
  
  return BCP_WATCH_ASSIGNMENT_WATCHED_ALL;
}

static void assignment_queue_init(assignment_queue_t *assignment_queue, size_t size)
{
  assignment_queue->assignments = malloc(sizeof(assignment_t *) * size);
  assignment_queue->size = 0;
}

static size_t assignment_queue_add(assignment_queue_t *queue, assignment_t *assignment)
{
  queue->assignments[queue->size] = assignment;
  return ++queue->size;
}
