#ifndef DPLL_H
#define DPLL_H

#include "formula.h"
#include "evaluation.h"
#include <stdbool.h>

#define DPLL_ASSIGNMENT_DONT_CARE false

/**
 Computes whether a formula is satisfiable.

 @param assignments If the formula is satisfiable, this array is populated with the
 assignments for each variable. The array should be large enough to store an assignment
 per variable in the formula.
 @return True if the formula is satisfiable, false otherwise.
 */
bool dpll(formula_t *formula, bool assignments[]);

#endif // !DPLL_H
