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
    queue->_capacity = capacity;
    queue->size = 0;

    queue->_elems = calloc(capacity, sizeof (variable_priority_queue_elem_t));
}

void variable_priority_queue_free(variable_priority_queue_t *queue)
{
    if (queue != NULL) free(queue->_elems);
}

static void _variable_priority_queue_resize(variable_priority_queue_t *queue, size_t capacity)
{
#ifdef NDEBUG
    fprintf(stderr, "%s: Vector resize from %d to %d.\n", __func__, queue->_capacity, capacity);
#endif

    variable_priority_queue_elem_t *elems =
        realloc(queue->_elems, capacity * sizeof (variable_priority_queue_elem_t));

    if (elems)
    {
        queue->_elems = elems;
        queue->_capacity = capacity;
    }
}

static void _variable_priority_queue_rebuild_down(variable_priority_queue_t *queue, variable_priority_queue_elem_t *elem)
{
    assert(elem != NULL);
    size_t max_child_index = 2 * elem->index + 1;
    if (queue->size > max_child_index)
    {
        size_t right_child_index = max_child_index + 1;

        if (queue->_elems[max_child_index].priority > queue->_elems[right_child_index].priority)
        {
            max_child_index = right_child_index;
        }
        variable_priority_queue_elem_t *max_child = &queue->_elems[max_child_index];
        if (elem->priority < max_child->priority)
        {
            variable_priority_queue_elem_t tmp = queue->_elems[max_child->index];
            queue->_elems[elem->index] = queue->_elems[max_child->index];
            queue->_elems[max_child->index] = tmp;
            _variable_priority_queue_rebuild_down(queue, max_child);
        }
    }
}

static void _variable_priority_queue_rebuild_up(variable_priority_queue_t *queue, variable_priority_queue_elem_t *elem)
{
    assert(elem != NULL);
    if (elem->index > 0)
    {
        size_t parent_index = (elem->index - 1) / 2;
        variable_priority_queue_elem_t *parent = queue->_elems + parent_index;
        if (parent->priority < elem->priority)
        {
            variable_priority_queue_elem_t tmp = queue->_elems[parent_index];
            queue->_elems[parent_index] = queue->_elems[elem->index];
            queue->_elems[elem->index] = tmp;

            _variable_priority_queue_rebuild_up(queue, parent);
        }
    }
}

void variable_priority_queue_enqueue(variable_priority_queue_t *queue, int value, size_t priority)
{

    if (queue->size == queue->_capacity)
    {
        printf("resizing\n");
        _variable_priority_queue_resize(queue, queue->_capacity * 2);
    }

    size_t index = queue->size++;
    if (&queue->_elems[index])
    {
        variable_priority_queue_elem_t *elem = &queue->_elems[index];
        elem->index = index;
        elem->value = value;
        elem->priority = priority;
        printf("hello\n");

        _variable_priority_queue_rebuild_up(queue, elem);
    }
}

int variable_priority_queue_dequeue(variable_priority_queue_t *queue)
{
    if (queue->size == 0) return 0;

    int max = queue->_elems->value;

    queue->_elems[0] = queue->_elems[--queue->size];

    _variable_priority_queue_rebuild_down(queue, queue->_elems);

    if (queue->size <= queue->_capacity / 4)
    {
        _variable_priority_queue_resize(queue, queue->_capacity / 2);
    }

    return max;
}

