#ifndef CLAUSE_H
#define CLAUSE_H

#include <stddef.h>

typedef struct
{
    size_t size;
    size_t _capacity;
    int *variables;
} clause_t;

/**
 Initialise a clause, and allocate all the necessary ressources on the heap

 @param clause pointer to the clause struct
*/
void clause_init(clause_t *clause);

/**
 Resize the underlying memory allocation in advance if size is known

 @param clause pointer to the clause struct
 @param capacity the new capacity of the underlying memory allocation
*/
void clause_reserve(clause_t *clause, size_t capacity);

/**
 Add a variable to the clause

 @param clause pointer to the clause struct
 @param var variable number, negative values indicate a negation
 @return position in the underlying container
*/
size_t clause_add_var(clause_t *clause, int var);

/**
 Remove a variable from the underlying container

 @param clause pointer to the clause struct
 @param index the index of the element to delete
*/
void clause_delete_var(clause_t *clause, size_t index);

/**
 Get a variable from the clause

 @param clause pointer to the clause struct
 @param index index of the element to inspect
 @return the variable, negative values indicate negation
*/
int clause_get_var(clause_t *clause, size_t index);

/**
 Toggle a variable from the clause

 @param clause pointer to the clause struct
 @param index index of the element to toggle
 @return the variables new value, negative values indicate negation
*/
int clause_toggle_var(clause_t *clause, size_t index);

/**
 Deallocate the clause and all associated ressources

 @param clause pointer to the clause struct
*/
void clause_free(clause_t *clause);

#endif // !CLAUSE_H
