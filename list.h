#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

struct list_elem_t;

typedef struct
{
    size_t size;
    struct list_elem_t *header;
    struct list_elem_t *footer;
} list_t;

void list_init(list_t *list);

size_t list_append(list_t *list, void *value);

void *list_get_at(list_t *list, int pos);

void list_remove_at(list_t *list, int pos);

#endif // !LIST_H
