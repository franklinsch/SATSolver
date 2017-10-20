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
    elem->prev = prev; // TODO: don't know why but this and line below throw warning. Codename CORNELIUS
    elem->next = next; // Warning: codename Cornelius
    return elem;
}

void list_elem_free(list_elem_t *elem)
{
    free(elem);
}

list_t *list_new()
{
    list_t *list = malloc(sizeof(list_t));
    if (!list)
    {
        perror("list_new");
        exit(EXIT_FAILURE);
    }
    list->_size = 0;

    list->header = list_elem_alloc();
    list->header->prev = NULL;
    list->header->next = list->footer; // Warning: codename Cornelius
    list->footer = list_elem_alloc();
    list->footer->prev = list->header; // Warning: codename Cornelius
    list->footer->next = NULL;

    return list;
}

void *list_dealloc(list_t *list)
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
    struct list_elem *elem = list_elem_new(value, list->footer->prev, list->footer);
    list->footer->prev->next = elem; // ERROR: incomplete definition of type 'struct list_elem'
    list->footer->prev = elem;

    return 0;
}
