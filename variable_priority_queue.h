#ifndef VARIABLE_PRIORITY_QUEUE_H
#define VARIABLE_PRIORITY_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

struct variable_priority_queue_elem_t;

typedef struct
{
    size_t size;

    // A pointer to the first element of the list.
    struct variable_priority_queue_elem_t *_header;

    // A pointer to the last element of the list.
    struct variable_priority_queue_elem_t *_footer;
} variable_priority_queue_t;

/**
 Initializes a priority queue of variable.

 @param list The priority queue to initialize.
 */
void variable_priority_queue_init(variable_priority_queue_t *list);

/**
 Frees the resources associated to the given priority queue.
 */
void variable_priority_queue_free(variable_priority_queue_t *list);

/**
 Appends the given index to the index list.

 @return The position of the appended element.
 */
size_t variable_priority_queue_enqueue(variable_priority_queue_t *list, size_t value);

/**
 Return the index at the given position.

 @param pos The position of the element to retrieve.
 @return The value at the given position.
 */
size_t variable_priority_queue_get_max(variable_priority_queue_t *list, size_t pos);

/**
 Remove the index at the given position.

 @param pos The position of the element to remove.
 */
void variable_priority_queue_dequeue(variable_priority_queue_t *list, size_t pos);

#endif // !VARIABLE_PRIORITY_QUEUE_H
