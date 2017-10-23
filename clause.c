#include "clause.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLAUSE_INIT_CAPACITY 4

void clause_init(clause_t *clause)
{
    clause->size = 0;
    clause->variables = malloc(CLAUSE_INIT_CAPACITY * sizeof (int));
    clause->_capacity = (sizeof clause->variables) / (sizeof (int));
}

// Internal API for resizing the array underpinning a clause
static void _clause_resize(clause_t *clause, size_t capacity)
{
#ifdef NDEBUG
    fprintf(stderr, "%s: vector resize from %d to %d.\n", __FUNCTION__, clause->capacity, capacity);
#endif

    int *vars = realloc(clause->variables, capacity * sizeof (int));
    if (vars)
    {
        clause->variables = vars;
        clause->_capacity = capacity;
    }
}

void clause_reserve(clause_t *clause, size_t capacity)
{
    _clause_resize(clause, capacity);
}

size_t clause_add_var(clause_t *clause, int var)
{
    // The array underlying array is full, we need more memory
    if(clause->_capacity == clause->size)
    {
        _clause_resize(clause, clause->size * 2);
    }
    clause->variables[clause->size] = var;
    return clause->size++;
}

void clause_delete_var(clause_t *clause, size_t index)
{
    if (index >= clause->size)
        return;

    // Shift all the remaining elements to the left
    int *dst = clause->variables + index;
    int *src = dst + 1;
    size_t amount = clause->size - index - 1;
    memmove(dst, src, amount);

    clause->size--;

    if (clause->size > 0 && clause->size <= clause->_capacity / 4)
        _clause_resize(clause, clause->_capacity / 2);
}

int clause_get_var(clause_t *clause, size_t index)
{
    if (index >= clause->size)
        return 0;

    return clause->variables[index];
}

int clause_toggle_var(clause_t *clause, size_t index)
{
    if (index >= clause->size)
        return 0;

    int var = clause->variables[index];
    clause->variables[index] = -var;

    return var;
}

void clause_free(clause_t *clause)
{
    free(clause->variables);
}
