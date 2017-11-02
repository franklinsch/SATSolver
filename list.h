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

/**
 Checks if the supplied pointer is already in the list. This function has shallow equality semantics.

 @param value The element to find.
*/
bool list_find(list_t *list, void *value);

// Iterator

typedef struct
{
    struct list_elem_t *_curr;
} list_iterator_t;

/**
 Get an iterator for the current list. Returns a malloced pointer to the iterator the caller must remember to free.
*/
list_iterator_t *list_get_iterator(list_t *list);

/**
 Returns whether the iterator has a next value.
*/
bool list_iterator_has_next(list_iterator_t *it);

/**
 Gets the next value in the iterator.
*/
void *list_iterator_next(list_iterator_t *it);

/**
 Returns the value in the iterator.
*/
void *list_iterator_get(list_iterator_t *it);

#endif // !LIST_H
