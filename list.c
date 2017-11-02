#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct list_elem_t
{
    void *value;
    struct list_elem_t *next;
} list_elem_t;

void list_init(list_t *list)
{
    list->size = 0;

    list->_header = NULL;
    list->_footer = NULL;
}

void list_free(list_t *list)
{
    list_elem_t *elem = list->_header;

    while (elem)
    {
        list_elem_t *next = elem->next;
        free(elem);
        elem = next;
    }
}

size_t list_append(list_t *list, void *value)
{
    list_elem_t *elem = malloc(sizeof (list_elem_t));
    elem->value = value;
    elem->next = NULL;

    if (list->_header == NULL)
    {
        list->_header = elem;
        list->_footer = elem;
    }
    else
    {
        list->_footer->next = elem;
        list->_footer = elem;
    }

    return list->size++;
}

void *list_get_at(list_t *list, size_t index)
{
    assert(index < list->size);
    list_elem_t *curr = list->_header;
    
    for (size_t i = 0; i < index; i++) curr = curr->next;
    
    return curr->value;
}

void list_remove_at(list_t *list, size_t pos)
{
    assert(pos < list->size);

    if (pos == 0)
    {
        list_elem_t *prev_header = list->_header;
        list->_header = prev_header->next;
        free(prev_header);
        list->size--;

        if (list->size == 0) list->_footer = NULL;
        return;
    }

    list_elem_t *prev = list->_header;
    
    for (size_t i = 0; i < pos - 1; i++) prev = prev->next;

    if (prev->next != list->_footer)
    {
        list_elem_t *prev_next = prev->next;
        prev->next = prev_next->next;
        free(prev_next);
    }
    else
    {
        // We are removing the footer.
        free(list->_footer);
        list->_footer = prev;
    }

    list->size--;
}

// Iterator

list_iterator_t *list_get_iterator(list_t *list)
{
    list_iterator_t *iterator = malloc(sizeof (list_iterator_t));
    iterator->_curr = list->_header;
    return iterator;
}

bool list_iterator_has_next(list_iterator_t *it)
{
    return it->_curr->next == NULL;
}

void list_iterator_next(list_iterator_t *it)
{
    assert(it->_curr != NULL);
    it->_curr = it->_curr->next;
}

void *list_iterator_get(list_iterator_t *it)
{
    assert(it->_curr != NULL);
    return it->_curr->value;
}

