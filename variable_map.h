#ifndef VARIABLE_MAP_H
#define VARIABLE_MAP_H

#include <stddef.h>

struct variable_map_entry_t;
/**
 Represents a hash map of variables to (void *).
*/
typedef struct variable_map_t
{
    struct variable_map_entry_t *_buckets;
    size_t _used_buckets;
    // Needs to be a power of 2
    size_t _num_buckets;
    float _load_factor;
    int _num_variables;
} variable_map_t;

/**
 Initialises a map of variables.

 @param num_buckets The number of buckets to use initially. A value of 0 indicates the default number.
 @param load_factor The maximum allowed load factor before a resize. A value of 0 indicates the default value.
 @param num_variables The total number of indexable variables
*/
void variable_map_init(variable_map_t *map, const size_t num_buckets, const float load_factor, const int num_variables);

/**
 Deallocate the supplied map.
*/
void variable_map_free(variable_map_t *map);

/**
 Add a mapping from the pecified variable to the specified value.

 @param variable The variable to use as the key for this mapping.
 @param value A pointer to the value to be mapped to.
*/
void variable_map_add(variable_map_t *map, const int variable, const void *value);

/**
 Get the value associated with the specified key.

 @param variable The variable to get the value for.
 @return Returns a NULL pointer if the variable is not present in the map, otherwise a pointer to the contained value
*/
void *variable_map_get(variable_map_t *map, const int variable);

/**
 Remove the mapping associated with the specified key.

 @param variable The variable to remove the mapping for.
*/
void variable_map_remove(variable_map_t *map, const int variable);

#endif // !VARIABLE_MAP_H
