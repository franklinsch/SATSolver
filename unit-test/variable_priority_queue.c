#include "check.h"
#include "variable_priority_queue.h"

#include <stdio.h>

int main(void)
{
    variable_priority_queue_t queue;
    variable_priority_queue_init(&queue, 3);

    variable_priority_queue_enqueue(&queue, 1, 1);
    variable_priority_queue_enqueue(&queue, 4, 4);
    variable_priority_queue_enqueue(&queue, -1, 31);

    // Check it's able to do resizing
    variable_priority_queue_enqueue(&queue, -4, 11);

    CHECK( queue.size == 4 );

    int val = variable_priority_queue_dequeue(&queue);
    CHECK( (int) val == -1 );

    val = variable_priority_queue_dequeue(&queue);
    CHECK( (int) val == -4 );

    val = variable_priority_queue_dequeue(&queue);
    CHECK( (int) val == 4 );

    val = variable_priority_queue_dequeue(&queue);
    CHECK( (int) val == 1 );

    CHECK( queue.size == 0 );
    return 0;
}
