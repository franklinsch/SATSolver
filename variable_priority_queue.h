#ifndef VARIABLE_PRIORITY_QUEUE_H
#define VARIABLE_PRIORITY_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

struct variable_priority_queue_elem_t;

typedef struct
{
    size_t capacity;
    size_t size;

    // A pointer to the first element of the list.
    struct variable_priority_queue_elem_t **_elems;

} variable_priority_queue_t;

/**
 Initializes a priority queue of variable.

 @param list The priority queue to initialize.
 */
void variable_priority_queue_init(variable_priority_queue_t *queue, size_t size);

/**
 Frees the resources associated to the given priority queue.
 */
void variable_priority_queue_free(variable_priority_queue_t *queue);

/**
 Appends the given index to the index list.
 */
void variable_priority_queue_enqueue(variable_priority_queue_t *queue, int value, size_t priority);

/**
 Remove the index at the given position.

 @param pos The position of the element to remove.
 */
int variable_priority_queue_dequeue(variable_priority_queue_t *queue);

#endif // !VARIABLE_PRIORITY_QUEUE_H
