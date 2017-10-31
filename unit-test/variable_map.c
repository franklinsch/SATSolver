#include "check.h"
#include "variable_map.h"

int main(void)
{
    variable_map_t map;
    variable_map_init(&map, 4);

    variable_map_add(&map, 1, (void *) 1);
    variable_map_add(&map, 4,(void *)  4);
    variable_map_add(&map, -1, (void *) -1);
    variable_map_add(&map, -4, (void *) -4);

    void *val = variable_map_get(&map, 1);
    CHECK( (int) val == 1 );

    val = variable_map_get(&map, 4);
    CHECK( (int) val == 4 );

    val = variable_map_get(&map, -1);
    CHECK( (int) val == -1 );

    val = variable_map_get(&map, -4);
    CHECK( (int) val == -4 );
    return 0;
}
