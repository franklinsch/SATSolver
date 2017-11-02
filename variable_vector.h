#ifndef VARIABLE_VECTOR_H
#define VARIABLE_VECTOR_H

#include <stddef.h>

/**
 Defines a dynamic array abstraction for variables
*/
typedef struct variable_vector_t
{
    size_t size;
    size_t _capacity;
    int *variables;
} variable_vector_t;

/**
 Initialise a variable_vector, and allocate all the necessary resources on the heap.
*/
void variable_vector_init(variable_vector_t *vector);

/**
 Resize the underlying memory allocation in advance if size is known.

 @param capacity The new capacity of the underlying memory allocation.
*/
void variable_vector_reserve(variable_vector_t *vector, size_t capacity);

/**
 Append a variable to the variable_vector. This function does not check if the variable is
 already in the variable_vector.

 @param var Variable index, negative values indicate a negation.
 @return position in the underlying container.
*/
size_t variable_vector_push_back(variable_vector_t *vector, int var);

/**
 Remove a variable from the variable_vector.

 @param index The index of the element to delete.
*/
void variable_vector_delete(variable_vector_t *vector, size_t index);

/**
 Get a variable from the variable_vector at the given index.

 @param index Index of the element to inspect.
 @return A reference to the variable, where a negative value indicates negation.
*/
int *variable_vector_get(const variable_vector_t *vector, size_t index);

/**
 Returns a pointer to the first element in the vector.
*/
int *variable_vector_cbegin(const variable_vector_t *vector);

/**
 Returns a pointer to 1 past the end of the vector.
*/
int *variable_vector_cend(const variable_vector_t *vector);

/**
 Deallocate the variable_vector and all associated ressources.
*/
void variable_vector_free(variable_vector_t *variable_vector);

#endif // !VARIABLE_VECTOR_H
