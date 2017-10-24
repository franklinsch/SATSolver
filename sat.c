#include "formula.h"
#include "parser.h"
#include "dpll.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("sat accepts only 1 argument which is the filename of the formula.\n");
        exit(EXIT_FAILURE);
    }

    formula_t *formula = parse_dimacs_file(argv[1]);

    // There is no formula for some reason we can not continue.
    if (!formula)
    {
        exit(EXIT_FAILURE);
    }
    
    int assignments[formula->num_variables];

    printf("%s\n", dpll(formula, &assignments) == EVALUATION_TRUE ? "SAT" : "UNSAT");

    // Free the formula and its associated clauses.
    formula_free(formula);
    free(formula);

    exit(EXIT_SUCCESS);
}
