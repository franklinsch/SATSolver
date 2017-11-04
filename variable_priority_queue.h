#ifndef VARIABLE_PRIORITY_QUEUE_H
#define VARIABLE_PRIORITY_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

struct variable_priority_queue_elem_t;

typedef struct
{
    size_t _capacity;
    size_t size;

    // A pointer to the first element of the queue.
    struct variable_priority_queue_elem_t *_elems;

} variable_priority_queue_t;

/**
 Initializes a priority queue of variable.

 @param queue The priority queue to initialize.
 */
void variable_priority_queue_init(variable_priority_queue_t *queue, size_t size);

/**
 Frees the resources associated to the given priority queue.
 */
void variable_priority_queue_free(variable_priority_queue_t *queue);

/**
 Inserts the given value according to its priority.
 */
void variable_priority_queue_enqueue(variable_priority_queue_t *queue, int value, size_t priority);

/**
 Dequeue value with maximum priority
 */
int variable_priority_queue_dequeue(variable_priority_queue_t *queue);

#endif // !VARIABLE_PRIORITY_QUEUE_H
