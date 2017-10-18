#include "parser.h"

#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void skip_comments(FILE *fp)
{
    int read;
    for (read = fgetc(fp); read == 'c'; read = fgetc(fp))
    {
        char *lineptr = NULL;
        size_t len;
        getline(&lineptr, &len, fp);
        free(lineptr);
    }
    ungetc(read, fp);
}

static formula_t *parse_problem_line(FILE *fp)
{
    char p;
    char cnf[4];
    unsigned num_variables;
    unsigned num_clauses;

    skip_comments(fp);

    fscanf(fp, "%c %s %u %u \n", &p, cnf, &num_variables, &num_clauses);

    if (p != 'p')
        return NULL;

    if (strncmp(cnf, "cnf", 3))
        return NULL;

    return alloc_formula(num_clauses, num_variables);
}

static int parse_clause(formula_t *formula, FILE *fp)
{
    int *variables = calloc(formula->num_variables, sizeof (int));

    //This loop is completely fucked, works by chance (the break when we reach 0)
    int var;
    fscanf(fp, "%d ", &var);
    for(; var != 0; fscanf(fp, "%d ", &var))
    {
        if (abs(var) > formula->num_variables)
        {
            fprintf(stderr, "Unknown variable: %d at clause %u\n", var, c);
            // Report the error to the caller
            return 1;
        }

        // Convert var to 0-based variable index, ternary operator handles the negative case
        var = var < 0 ? var++ : var--;
        variables[abs(var)] = var;
    }

    add_clause(formula, variables);
    return 0;
}

formula_t *parse_dimacs_file(char *path)
{
    // Need to decalre this here in case the fopen call fails so we have smth to return
    formula_t * formula = NULL;

    FILE *fp = fopen(path, "r");

    if (!fp)
        goto cleanup;

    formula = parse_problem_line(fp);

    if (!formula)
        goto cleanup;

    for (unsigned c = 0; c < formula->num_clauses; ++c)
    {
        skip_comments(fp);

        if(parse_clause(formula, fp))
            goto cleanup;
    }

cleanup:
    fclose(fp);
    return formula;
}
