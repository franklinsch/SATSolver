#include "variable_map.h"

#include <assert.h>
#include <stdlib.h>


typedef struct variable_map_entry_t
{
    const void *value;
} variable_map_entry_t;

static size_t _hash_variable(variable_map_t *map, const int variable)
{
    return variable > 0 ? variable : (abs(variable) + map->_num_variables);
}

static void _free_bucket(variable_map_entry_t *bucket)
{
    assert(bucket->variable == BUCKET_HEADER_KEY);

    variable_map_entry_t *curr_entry = bucket->_next_entry;
    bucket->_next_entry = NULL;

    while (curr_entry != NULL)
    {
        variable_map_entry_t *next = curr_entry->_next_entry;
        free(curr_entry);
        curr_entry = next;
    }
}

static variable_map_entry_t* _search_bucket(const int variable, const variable_map_entry_t *bucket)
{
    assert(bucket->variable == BUCKET_HEADER_KEY);

    variable_map_entry_t *it = bucket->_next_entry;

    for (; (it != NULL) && (it->variable != variable); it = it->_next_entry);

    return it;
}

static void _rehash(variable_map_t *map)
{
    size_t num_buckets = map->_num_buckets;
    variable_map_entry_t *buckets = map->_buckets;

    variable_map_init(map, num_buckets * 2, map->_load_factor, map->_num_variables);

    variable_map_entry_t *buckets_end = buckets + num_buckets;
    for (variable_map_entry_t *it; it < buckets_end; it++)
    {
        assert(it->variable == BUCKET_HEADER_KEY);
        for (variable_map_entry_t *pair = it->_next_entry; pair != NULL; pair = pair->_next_entry)
        {
            // What happens if after rehash we exceed the load factor? Should be fine, but...
            variable_map_add(map, pair->variable, pair->value);
        }

        _free_bucket(it);
    }

    free(buckets);

}

void variable_map_init(variable_map_t *map, const size_t num_buckets, const float load_factor, const int num_variables)
{
    map->_num_variables = num_variables;

    map->_buckets = calloc(map->_num_variables << 1, sizeof (variable_map_entry_t));
}

void variable_map_free(variable_map_t *map)
{
    free(map->_buckets);
}

void variable_map_add(variable_map_t *map, const int variable, const void *value)
{
    // Mod just to be sure
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) % (map->_num_variables << 1));

    bucket->value = value;
}

void *variable_map_get(variable_map_t *map, const int variable)
{
    // Mod just to be sure
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) % (map->_num_variables << 1));

    // Explicitly remove constness of the pointer for the user to do whatever they want with their memory
    return (void *) bucket->value;
}

void variable_map_remove(variable_map_t *map, const int variable)
{
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) & (map->_num_buckets - 1));

    assert(bucket->variable == BUCKET_HEADER_KEY);

    variable_map_entry_t *prev_entry = bucket;

    for (variable_map_entry_t *it = bucket->_next_entry; it != NULL; it = it->_next_entry)
    {
        if (it->variable == variable)
        {
            prev_entry->_next_entry = it->_next_entry;
            free(it);
            return;
        }

        prev_entry = it;
    }
}
