#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARIABLE_VECTOR_INIT_CAPACITY 4

void vector_init(vector_t *vector)
{
    vector->size = 0;
    vector->elems = malloc(VARIABLE_VECTOR_INIT_CAPACITY * sizeof (void *));
    vector->_capacity = VARIABLE_VECTOR_INIT_CAPACITY;
}

// Internal API for resizing the array underpinning a vector.
static void _vector_resize(vector_t *vector, size_t capacity)
{
#ifdef LOGGING
    fprintf(stderr, "%s: Vector resize from %d to %d.\n", __func__, vector->_capacity, capacity);
#endif

    if (capacity == 0) return;

    void **vars = realloc(vector->elems, capacity * sizeof (void *));
    if (vars)
    {
        vector->elems = vars;
        vector->_capacity = capacity;
    }
}

void vector_reserve(vector_t *vector, size_t capacity)
{
    if (capacity > vector->size) _vector_resize(vector, capacity);
}

size_t vector_push_back(vector_t *vector, void *elem)
{
    // The array underlying array is full, we need more memory.
    if(vector->_capacity == vector->size)
    {
        _vector_resize(vector, vector->size * 2);
    }
    vector->elems[vector->size++] = elem;
    return vector->size;
}

void vector_delete_ptr(vector_t *vector, void *target)
{
    void **elem = vector_find(vector, target);

    while (elem)
    {
        vector_delete(vector, elem - vector->elems);
        elem = vector_find(vector, target);
    }
}

void **vector_delete(vector_t *vector, size_t index)
{
    assert(index < vector->size);
    assert(vector->size > 0);

    // Shift all the remaining elements one slot to the left.
    void **dst = vector->elems + index;
    void **src = dst + 1;
    size_t amount = vector->size - index - 1;
    memmove(dst, src, amount * sizeof(void *));

    vector->size--;

    size_t offset = src - vector->elems;

    if (vector->size != 0 && vector->size <= vector->_capacity / 4)
    {
        _vector_resize(vector, vector->_capacity / 2);
    }

    return vector->elems + offset;
}

void **vector_get(const vector_t *vector, size_t index)
{
    assert(index < vector->size);

    return vector->elems + index;
}

void **vector_find(const vector_t *vector, void *value)
{
    for (void **elem = vector->elems; elem < vector_cend(vector); elem++)
    {
        if (*elem == value) return elem;
    }
    return NULL;
}

void **vector_cbegin(const vector_t *vector)
{
    return vector->elems;
}

void **vector_cend(const vector_t *vector)
{
    return vector->elems + vector->size;
}

void vector_free(vector_t *vector)
{
    free(vector->elems);
}
