#ifndef BCP_H
#define BCP_H

#include <stdio.h>

#include "formula.h"

/**
 Updates the given implication graph by deducing necessary assignments.

 @param formula The formula to apply BCP on.
 @param curr_assignment The node representing the latest assignment made by the caller.
 */
void bcp(const formula_t *formula, implication_graph_node_t *curr_assignment);

#endif // !BCP_H
