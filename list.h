#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

struct list_elem_t;

typedef struct
{
    size_t size;

    // A pointer to the first element of the list.
    struct list_elem_t *_header;

    // A pointer to the last element of the list.
    struct list_elem_t *_footer;
} list_t;

/**
 Initializes an index list.

 @param list The list to initialize.
 */
void list_init(list_t *list);

/**
 Frees the resources associated to the given list.
 */
void list_free(list_t *list);

/**
 Appends the given value to the list.

 @return The position of the appended element.
 */
size_t list_append(list_t *list, void *value);

/**
 Return the value at the given position.

 @param pos The position of the element to retrieve.
 @return The value at the given position.
 */
void *list_get_at(list_t *list, size_t pos);

/**
 Remove the value at the given position.

 @param pos The position of the element to remove.
 */
void list_remove_at(list_t *list, size_t pos);

#endif // !LIST_H
