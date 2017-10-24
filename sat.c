#include "formula.h"
#include "parser.h"
#include "dpll.h"

#include <stdio.h>
#include <stdlib.h>

static void print_assignments(int assignments[], int num_variables)
{
    for (int i = 0; i < num_variables; i++)
    {
        if (assignments[i] == DPLL_ASSIGNMENT_DONT_CARE)
        {
            printf("%d ", i + 1);
        }
        else
        {
            printf("%d ", assignments[i]);
        }
    }
    printf("\n");
}

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

    bool is_sat = dpll(formula, assignments);
    
//    formula_print(formula);
    
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
