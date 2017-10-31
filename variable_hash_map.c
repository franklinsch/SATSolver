#include "variable_hash_map.h"

#include <assert.h>
#include <stdlib.h>

#define DEFAULT_NUM_BUCKETS 16
#define DEFAULT_LOAD_FACTOR 1.0f
// A variable cannot be 0 so we use this value to signify the start of the bucket list info
#define BUCKET_HEADER_KEY 0

typedef struct variable_map_entry_t
{
    int variable;
    const void *value;
    struct variable_map_entry_t *_next_entry;
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
    map->_used_buckets = 0;
    map->_num_buckets = num_buckets ? num_buckets : DEFAULT_NUM_BUCKETS;
    map->_load_factor = load_factor == 0.0f ? load_factor : DEFAULT_LOAD_FACTOR;

    map->_buckets = malloc(map->_num_buckets * sizeof (variable_map_entry_t));

    variable_map_entry_t *buckets_end = map->_buckets + map->_num_buckets;
    for (variable_map_entry_t *bucket = map->_buckets; bucket < buckets_end; bucket++)
    {
        bucket->variable = BUCKET_HEADER_KEY;
        // Terminator for bucket chain
        bucket->_next_entry = NULL;
    }
}

void variable_map_free(variable_map_t *map)
{
    variable_map_entry_t *buckets_end = map->_buckets + map->_num_buckets;
    for (variable_map_entry_t *bucket = map->_buckets; bucket < buckets_end; bucket++)
    {
        assert(bucket->variable == BUCKET_HEADER_KEY);
        _free_bucket(bucket);
    }

    free(map->_buckets);
}

void variable_map_add(variable_map_t *map, const int variable, const void *value)
{
    // If the load is too high we need to resize
    if (map->_used_buckets / map->_num_buckets > map->_load_factor)
    {
        _rehash(map);
    }

    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) & (map->_num_buckets - 1));
    assert(bucket->variable == BUCKET_HEADER_KEY);

    variable_map_entry_t *fst_entry = bucket->_next_entry;
    if (fst_entry == NULL)
    {
        map->_used_buckets++;
    }

    variable_map_entry_t *new_entry = malloc(sizeof (variable_map_entry_t));
    new_entry->variable = variable;
    new_entry->value = value;
    new_entry->_next_entry = fst_entry;
    bucket->_next_entry = new_entry;
}

void *variable_map_get(variable_map_t *map, const int variable)
{
    variable_map_entry_t *bucket = map->_buckets + (_hash_variable(map, variable) & (map->_num_buckets - 1));
    variable_map_entry_t *entry = _search_bucket(variable, bucket);
    return (void *) entry->value;
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
