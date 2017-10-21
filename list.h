#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct list_elem_t
{
    int value;
    struct list_elem_t *prev;
    struct list_elem_t *next;
} list_elem_t;

typedef struct
{
    size_t _size;
    list_elem_t *header;
    list_elem_t *footer;
} list_t;

list_elem_t *list_elem_alloc();

void list_elem_free();

list_t *list_new();

bool is_empty(list_t *list);

size_t size(list_t *list);

size_t list_append(list_t *list, int value);

list_elem_t* list_get_at(list_t *list, size_t pos);

void list_remove_at(list_t *list, size_t pos);

#endif // !LIST_H
