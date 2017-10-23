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
    int length;
    list_elem_t *header;
    list_elem_t *footer;
} list_t;

void list_init(list_t *list);

int list_length(list_t *list);

int list_append(list_t *list, int value);

int list_get_at(list_t *list, int pos);

void list_remove_at(list_t *list, int pos);

#endif // !LIST_H
