#include "check.h"
#include "variable_map.h"

int main(void)
{
    variable_map_t map;
    // Low number of buckets and default load factor to force a resize
    variable_map_init(&map, 4, 0.5f, 4);

    variable_map_add(&map, 1, (void *) 1);
    variable_map_add(&map, 4, (void *)  4);
    // The next statement will trigger a resize and hash
    variable_map_add(&map, -1, (void *) -1);
    variable_map_add(&map, -4, (void *) -4);
    variable_map_add(&map, -4, (void *) -3);

    void *val = variable_map_get(&map, 1);
    CHECK( (int) val == 1 );

    val = variable_map_get(&map, 4);
    CHECK( (int) val == 4 );

    val = variable_map_get(&map, -1);
    CHECK( (int) val == -1 );

    val = variable_map_get(&map, -4);
    CHECK( (int) val == -3 );

    variable_map_free(&map);
    return 0;
}
