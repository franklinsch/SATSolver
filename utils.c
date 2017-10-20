#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int clause_id;
    struct chain_elem *next;
} chain_elem_t;

typedef struct
{
    struct chain_elem *head;
} chain_t;


chain_elem_t *chain_elem_alloc() {
    chain_elem_t *elem = malloc(sizeof(chain_elem_t));
    if (elem == NULL) {
        perror("Error allocating memory for chain element in hashmap");
        exit(EXIT_FAILURE);
    }

    return elem;
}

void chain_elem_free(chain_elem_t *elem) {
    free(elem);
}

typedef chain_t* hash_map;

bool isEmpty(hash_map *map);

unsigned int size(hash_map *map);

void add_elem(hash_map *map, int key, int value);

void remove_elem(hash_map *map, int key);

int get_value(hash_map *map, int key);
