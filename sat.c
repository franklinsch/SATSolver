#include "formula.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("sat accepts only 1 argument which is the filename of the formula.\n");
        exit(EXIT_FAILURE);
    }

    formula_t *formula = parse_dimacs_file(argv[1]);

    print_formula(formula);

    dealloc_formula(formula);

    printf("UNSAT\n");

    exit(EXIT_SUCCESS);
}
