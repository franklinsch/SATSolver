#include "variable_priority_queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct variable_priority_queue_elem_t
{
    int value;
    size_t priority;
    size_t index;
    /* struct variable_priority_queue_elem_t *left; */
    /* struct variable_priority_queue_elem_t *right; */
} variable_priority_queue_elem_t;

void variable_priority_queue_init(variable_priority_queue_t *queue, size_t capacity)
{
    queue->capacity = capacity;
    queue->size = 0;

    queue->_elems = malloc(capacity * sizeof (variable_priority_queue_elem_t *));
}

void _variable_priority_queue_elem_free(variable_priority_queue_elem_t *elem)
{
    if (elem != NULL)
    {
        free(elem);
    }
}

void variable_priority_queue_free(variable_priority_queue_t *queue)
{
    if (queue != NULL)
    {
        _variable_priority_queue_elem_free(queue->_elems);
    }

    free(queue);
}

void _variable_priority_queue_rebuild_down(variable_priority_queue_t *queue, variable_priority_queue_elem_t *elem)
{
    assert(elem != NULL);
    size_t left_child_index = 2 * elem->index + 1;
    if (queue->size > left_child_index)
    {
        variable_priority_queue_elem_t *max_child = queue->_elems[left_child_index];
        size_t right_child_index = left_child_index + 1;

        if (queue->size > right_child_index)
        {
            max_child = elem + right_child_index;
        }

        if (elem->priority < max_child->priority)
        {
            variable_priority_queue_elem_t *tmp = elem;
            elem = max_child;
            max_child = tmp;

            _variable_priority_queue_heap_rebuild(queue, max_child);
        }
    }
}

void _variable_priority_queue_rebuild_up(variable_priority_queue_t *queue, variable_priority_queue_elem_t *elem)
{
    assert(elem != NULL);
    if (elem->index > 0)
    {
        size_t parent_index = (elem->index - 1) / 2;
        variable_priority_queue_elem_t *parent = queue->_elems + parent_index;
        if (parent->priority < elem->priority)
        {
            variable_priority_queue_elem_t *tmp = parent;
            parent = elem;
            elem = parent;

            _variable_priority_queue_rebuild_up(queue, parent);
        }
    }
}

void variable_priority_queue_enqueue(variable_priority_queue_t *queue, int value, size_t priority)
{

    //TODO: resize if size == capacity


    variable_priority_queue_elem_t *elem = queue->_elems[queue->size];
    elem->value = value;
    elem->priority = priority;
    elem->index = queue->size;
    _variable_priority_queue_rebuild_up(queue, elem);
}

int variable_priority_queue_dequeue(variable_priority_queue_t *queue)
{
    if (queue->size == 0) return 0;

    int max = queue->_elems[0]->value;

    queue->_elems[0] = queue->_elems[queue->size - 1];
    queue->size--;
    _variable_priority_queue_heap_rebuild(queue, queue->_elems[0]);

    //TODO: resize if little size compared to capacity

    return max;
}
