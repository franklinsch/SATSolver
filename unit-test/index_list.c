#include "check.h"
#include "index_list.h"

int main(void)
{
    index_list_t list;
    index_list_init(&list);

    size_t values[] = { 3902, -348, 0 };

    for (size_t i = 0; i < 3; i++)
    {
        index_list_append(&list, values[i]);
    }

    for (size_t i = 0; i < 3; i++)
    {
        size_t val = index_list_get_at(&list, i);
        CHECK( val == values[i] );
    }


    for (size_t i = 0; i < 3; i++)
    {
        size_t val = index_list_get_at(&list, 0);
        CHECK( val == values[i] );

        index_list_remove_at(&list, 0);
    }

    index_list_free(&list);

    return 0;
}
