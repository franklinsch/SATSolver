#include "index_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// MARK: Internal data structures

typedef struct list_elem_t
{
    size_t value;
    struct list_elem_t *next;
} list_elem_t;

// MARK: Functions

void index_list_init(index_list_t *list)
{
    list->size = 0;

    list->_header = NULL;
    list->_footer = NULL;
}

void index_list_free(index_list_t *list)
{
    list_elem_t *elem = list->_header;

    while (elem)
    {
        list_elem_t *next = elem->next;
        free(elem);
        elem = next;
    }
}

size_t index_list_append(index_list_t *list, size_t value)
{
    list_elem_t *elem = malloc(sizeof (list_elem_t));
    elem->value = value;
    elem->next = list->_footer;

    if (list->_header == NULL)
    {
        list->_header = elem;
        list->_footer = elem;
        return list->size++;
    }

    list->_footer->next = elem;
    list->_footer = elem;

    return list->size++;
}

size_t index_list_get_at(index_list_t *list, size_t index)
{
    assert(index < list->size);
    list_elem_t *curr = list->_header;
    
    for (size_t i = 0; i < index; i++) { curr = curr->next; };
    
    return curr->value;
}

void index_list_remove_at(index_list_t *list, size_t pos)
{
    assert(pos < list->size);

    if (pos == 0)
    {
        list_elem_t *prev_header = list->_header;
        list->_header = prev_header->next;
        free(prev_header);
        list->size--;
        return;
    }

    list_elem_t *prev = list->_header;
    
    for (size_t i = 0; i < pos - 1; i++) prev = prev->next;

    if (prev->next != NULL)
    {
        list_elem_t *prev_next = prev->next;
        prev->next = prev_next->next;
        free(prev_next);
    }
    else
    {
        free(list->_footer);
        list->_footer = prev;
    }

    list->size--;
}
