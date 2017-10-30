#include "variable_map.h"

#include <assert.h>
#include <stdlib.h>


typedef struct variable_map_entry_t
{
    int variable;
    const void *value;
} variable_map_entry_t;

static size_t _hash_variable(variable_map_t *map, const int variable)
{
    return variable < 0 ? variable : (abs(variable) + map->_num_variables);
}

void variable_map_init(variable_map_t *map, const int num_variables)
{
    map->_num_variables = num_variables;

    map->_buckets = calloc(map->_num_variables, sizeof (variable_map_entry_t));
}

void variable_map_free(variable_map_t *map)
{
    free(map->_buckets);
}

void variable_map_add(variable_map_t *map, const int variable, const void *value)
{
    // Mod just to be sure
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) % map->_num_variables);

    bucket->variable = variable;
    bucket->value = value;
}

void *variable_map_get(variable_map_t *map, const int variable)
{
    // Mod just to be sure
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) % map->_num_variables);

    // Explicitly remove constness of the pointer for the user to do whatever they want with their memory
    return (void *) bucket->value;
}

void variable_map_remove(variable_map_t *map, const int variable)
{
    // Mod just to be sure
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) % map->_num_variables);
    bucket->variable = 0;
    bucket->value = NULL;
}
