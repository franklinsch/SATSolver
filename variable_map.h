#ifndef DPLL_H
#define DPLL_H

typedef strcut variable_map_t
{
    void **values;
    size_t num_values;
}

/**
 Initialises a map of variables.

 @param variables The number of variables in the formula, which is half the number of possible keys
*/
void variable_map_init(variable_map_t *map, size_t num_tvariables);

/**
 Add a mapping from the pecified variable to the specified value.

 @param variable The variable to use as the key for this mapping.
 @param value A pointer to the value to be mapped to.
*/
void variable_map_add(variable_map_t *map, int variable, void *value);

/**
 Get the value associated with the specified key.

 @param variable The variable to get the value for.
*/
void variable_map_get(variable_map_t *map, int variable);

/**
 Remove the mapping associated with the specified key.

 @param variable The variable to remove the mappingfor.
*/
void variable_map_remove(variable_map_t *map, int variable);

#endif // !DPLL_H
