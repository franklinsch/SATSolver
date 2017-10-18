#ifndef CLAUSE_H
#define CLAUSE_H

#include <stddef.h>

typedef struct
{
    size_t size;
    size_t capacity;
    int *variables;
} clause_t;

/*
 * Initialise a clause
 */
int clause_init(clause_t *clause);

/*
 * Adding and removing variables from the clause
 */
size_t clause_add_var(clause_t *clause, int var);
void clause_delete_var(clause_t *clause, size_t index);

/*
 * Examining existing variables
 */
int clause_get_var(clause_t *clause, size_t index);
int clause_toggle_var(clause_t *clause, size_t index);

/*
 * Free all ressources associated to a clause
 */
void clause_free(clause_t *clause);

#endif // !CLAUSE_H
