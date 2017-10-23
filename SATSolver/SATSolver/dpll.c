#include "dpll.h"
#include "formula.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t
{
    int depth;
    
    int *assignments;
    size_t num_assignments;
    
    struct node_t *parents;
    size_t num_parents;
    
    struct node_t *children;
    size_t num_children;
} node_t;

bool dpll(formula_t *formula)
{
    return false;
}
