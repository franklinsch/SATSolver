#ifndef INDEX_LIST_H
#define INDEX_LIST_H

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
} index_list_t;

/**
 Initializes an index list.

 @param list The list to initialize.
 */
void index_list_init(index_list_t *list);

/**
 Frees the resources associated to the given list.
 */
void list_free(index_list_t *list);

/**
 Appends the given index to the index list.

 @return The position of the appended element.
 */
size_t index_list_append(index_list_t *list, size_t value);

/**
 Return the index at the given position.

 @param pos The position of the element to retrieve.
 @return The value at the given position.
 */
size_t index_list_get_at(index_list_t *list, size_t pos);

/**
 Remove the index at the given position.

 @param pos The position of the element to remove.
 */
void index_list_remove_at(index_list_t *list, size_t pos);

#endif // !INDEX_LIST_H
