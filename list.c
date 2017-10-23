#include "list.h"

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

list_elem_t *list_elem_alloc()
{
    list_elem_t *elem = malloc(sizeof(list_elem_t));
    if (elem == NULL)
    {
        perror("list_elem_alloc");
        exit(EXIT_FAILURE);
    }

    return elem;
}

list_elem_t *list_elem_new(int value, list_elem_t *prev, list_elem_t *next)
{
    list_elem_t *elem = list_elem_alloc();
    elem->value = value;
    elem->prev = prev; 
    elem->next = next;
    return elem;
}

void list_elem_free(list_elem_t *elem)
{
    free(elem);
}

void list_init(list_t *list)
{
    list->_size = 0;

    list->header = list_elem_alloc();
    list->header->prev = NULL;
    list->header->next = list->footer;
    list->footer = list_elem_alloc();
    list->footer->prev = list->header;
    list->footer->next = NULL;
}

void list_dealloc(list_t *list)
{
    if (list)
    {
        list_elem_t *elem = list->header;
        while(elem)
        {
            list_elem_t *next = elem->next;
            list_elem_free(elem);
            elem = next;
        }
    }
}

bool is_empty(list_t *list)
{
    return list->_size == 0;
}

size_t size(list_t *list)
{
    return list->_size;
}

size_t list_append(list_t *list, int value)
{
    list_elem_t *elem = list_elem_new(value, list->footer->prev, list->footer);
    list->footer->prev->next = elem;
    list->footer->prev = elem;

    return 0;
}
