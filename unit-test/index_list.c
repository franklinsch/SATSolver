#include "check.h"
#include "list.h"

int main(void)
{
    list_t list;
    list_init(&list);

    size_t values[] = { 3902, -348, 0 };

    for (size_t i = 0; i < 3; i++)
    {
        list_append(&list, (void *) values[i]);
    }

    for (size_t i = 0; i < 3; i++)
    {
        size_t val = (size_t) list_get_at(&list, i);
        CHECK( val == values[i] );
    }


    for (size_t i = 0; i < 3; i++)
    {
        size_t val = (size_t) list_get_at(&list, 0);
        CHECK( val == values[i] );

        list_remove_at(&list, 0);
    }

    list_free(&list);

    return 0;
}
