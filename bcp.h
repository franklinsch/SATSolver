#ifndef BCP_H
#define BCP_H

#include "formula.h"
#include "implication_graph.h"
#include "evaluation.h"

/**
 Initialises the internal BCP data structures.

 @param formula The formula on which we will be performing the BCP.
 @param root the root of the implication graph to use.
 @return An EVALUATION that determines trivially decidable formulas.
*/
EVALUATION bcp_init(const formula_t *formula, implication_graph_node_t *root);

/**
 Performs BCP on the given formula.

 @param node The last assignment that was made by the DPLL procedure.
*/
EVALUATION bcp(implication_graph_node_t *node);

/**
 Frees the resources BCP needs.
*/
void bcp_free(void);

#endif // !BCP_H
