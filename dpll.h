#ifndef DPLL_H
#define DPLL_H

#include "formula.h"
#include <stdbool.h>

bool dpll(formula_t *formula, int assignments[]);

#endif // !DPLL_H
