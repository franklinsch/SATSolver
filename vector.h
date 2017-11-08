#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/**
 Defines a dynamic array abstraction for (void *)
*/
typedef struct vector_t
{
    size_t size;
    size_t _capacity;
    void **elems;
} vector_t;

/**
 Initialise a vector, and allocate all the necessary resources on the heap.
*/
void vector_init(vector_t *vector);

/**
 Resize the underlying memory allocation in advance if size is known.

 @param capacity The new capacity of the underlying memory allocation.
*/
void vector_reserve(vector_t *vector, size_t capacity);

/**
 Append a variable to the vector. This function does not check if the variable is
 already in the vector.

 @param elem Variable index, negative values indicate a negation.
 @return position in the underlying container.
*/
size_t vector_push_back(vector_t *vector, void *elem);

/**
 Remove the value at the given target pointer in the given vector, using referential equality.

 @param target The entry to delete.
 */
void vector_delete_ptr(vector_t *vector, void *target);

/**
 Remove a variable from the vector.

 @param index The index of the element to delete.
*/
void **vector_delete(vector_t *vector, size_t index);

/**
 Get a variable from the vector at the given index.

 @param index Index of the element to inspect.
 @return A reference to the variable, where a negative value indicates negation.
*/
void **vector_get(const vector_t *vector, size_t index);

/**
 Finds the specified value in the specified vector. This uses shallow equality semantics.

 @param value The value to search for.
 @return A reference to the element, if it exists, otherwise NULL is returned.
*/
void **vector_find(const vector_t *vector, void *value);

/**
 Returns a pointer to the first element in the vector.
*/
void **vector_cbegin(const vector_t *vector);

/**
 Returns a pointer to 1 past the end of the vector.
*/
void **vector_cend(const vector_t *vector);

/**
 Deallocate the vector and all associated ressources.
*/
void vector_free(vector_t *vector);

#endif // !VECTOR_H
