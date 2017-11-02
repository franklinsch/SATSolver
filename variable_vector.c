#include "variable_vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARIABLE_VECTOR_INIT_CAPACITY 4

void variable_vector_init(variable_vector_t *vector)
{
    vector->size = 0;
    vector->variables = malloc(VARIABLE_VECTOR_INIT_CAPACITY * (sizeof (int)));
    vector->_capacity = (sizeof vector->variables) / (sizeof (int));
}

// Internal API for resizing the array underpinning a vector.
static void _variable_vector_resize(variable_vector_t *vector, size_t capacity)
{
#ifdef LOGGING
    fprintf(stderr, "%s: Vector resize from %d to %d.\n", __func__, vector->_capacity, capacity);
#endif

    int *vars = realloc(vector->variables, capacity * sizeof (int));
    if (vars)
    {
        vector->variables = vars;
        vector->_capacity = capacity;
    }
}

void variable_vector_reserve(variable_vector_t *vector, size_t capacity)
{
    _variable_vector_resize(vector, capacity);
}

size_t variable_vector_push_back(variable_vector_t *vector, int var)
{
    // The array underlying array is full, we need more memory.
    if(vector->_capacity == vector->size)
    {
        _variable_vector_resize(vector, vector->size * 2);
    }
    vector->variables[vector->size++] = var;
    return vector->size;
}

void variable_vector_delete(variable_vector_t *vector, size_t index)
{
    assert(index < vector->size && vector->size > 0);

    // Shift all the remaining elements one slot to the left.
    int *dst = vector->variables + index;
    int *src = dst + 1;
    size_t amount = vector->size - index - 1;
    memmove(dst, src, amount);

    vector->size--;

    if (vector->size <= vector->_capacity / 4)
        _variable_vector_resize(vector, vector->_capacity / 2);
}

int *variable_vector_get(const variable_vector_t *vector, size_t index)
{
    assert(index >= vector->size);

    return vector->variables + index;
}

int *variable_vector_cbegin(const variable_vector_t *vector)
{
    return vector->variables;
}

int *variable_vector_cend(const variable_vector_t *vector)
{
    return vector->variables + vector->size;
}

void variable_vector_free(variable_vector_t *vector)
{
    free(vector->variables);
}
