#ifndef BCP_H
#define BCP_H

#include "formula.h"
#include "implication_graph.h"
#include "evaluation.h"
#include "variable_map.h"

/**
 Initialises the internal BCP data structures.

 @param formula The formula on which we will be performing the BCP.
 @param implication_graph The current implication graph.
 @return An EVALUATION that determines trivially decidable formulas.
*/
EVALUATION bcp_init(formula_t *formula, implication_graph_t *implication_graph);

/**
 Performs BCP on the given formula.

 @param implication_graph The current implication graph.
 @param last_assignment The last assignment that was made by the DPLL procedure.
 @param decision_level The current decision level.
*/
void bcp(implication_graph_t *implication_graph, int last_assignment, size_t decision_level);

/**
 Frees the resources BCP needs.
*/
void bcp_free(void);

#endif // !BCP_H
