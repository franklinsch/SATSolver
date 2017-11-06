#include "formula.h"
#include "parser.h"
#include "dpll.h"

#include <stdio.h>
#include <stdlib.h>

static void print_assignments(bool assignments[], int num_variables)
{
    for (int variable = 1; variable <= num_variables; variable++)
    {
        // If the assignment is negative, make the variable negative.
        int value = (assignments[variable - 1] ? 1 : -1) * variable;
        printf("%d%s", value, variable < num_variables ? " " : "");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("sat accepts only 1 argument which is the filename of the formula.\n");
        exit(EXIT_FAILURE);
    }

    formula_t *formula = parse_dimacs_file(argv[1]);

    if (!formula) exit(EXIT_FAILURE);

    bool assignments[formula->num_variables];

    bool is_sat = dpll(formula, assignments);

    if (is_sat)
    {
        printf("SAT\n");
        print_assignments(assignments, formula->num_variables);
    }
    else
    {
        printf("UNSAT\n");
    }

    // Free the formula and its associated clauses.
    formula_free(formula);
    free(formula);

    exit(EXIT_SUCCESS);
}
