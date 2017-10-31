#ifndef INDEX_LIST_H
#define INDEX_LIST_H

#include <stddef.h>
#include <stdbool.h>

struct list_elem_t;

typedef struct
{
    size_t size;
    struct list_elem_t *header;
    struct list_elem_t *footer;
} list_t;

void index_list_init(list_t *list);

size_t index_list_append(list_t *list, void *value);

void *index_list_get_at(list_t *list, int pos);

void index_list_remove_at(list_t *list, int pos);

#endif // !INDEX_LIST_H
