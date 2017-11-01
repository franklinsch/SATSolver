#ifndef BCP_H
#define BCP_H

#include "formula.h"
#include "implication_graph.h"

/**
 Initialises the internal BCP data structures.

 @param formula The formula on which we will be performing the BCP.
*/
void bcp_init(const formula_t *formula);

/**
 Performs BCP on the given formula.

 @param node, The last assignment that was made by the DPLL procedure.
*/
void bcp(implication_graph_node_t *node);

/**
 Frees the resources BCP needs.
*/
void bcp_free();

#endif // !BCP_H
