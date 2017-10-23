#include "parser.h"

#include "formula.h"
#include "clause.h"

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

    formula_t *formula = malloc(sizeof (formula_t));
    formula_init(formula, num_clauses, num_variables);
    return formula;
}

static int parse_clause(formula_t *formula, FILE *fp, unsigned clause_index)
{
    clause_t clause;
    clause_init(&clause);

    int var;
    for(fscanf(fp, "%d ", &var); var != 0; fscanf(fp, "%d ", &var))
    {
        if (abs(var) > formula->num_variables)
        {
            fprintf(stderr, "Unknown variable: %d at clause %u\n", var, clause_index);
            // Report the error to the caller
            return 1;
        }

        clause_add_var(&clause, var);
    }

    formula_add_clause(formula, clause);
    return 0;
}

formula_t *parse_dimacs_file(char *path)
{
    FILE *fp = fopen(path, "r");

    if (!fp)
        return NULL;

    formula_t *formula = parse_problem_line(fp);

    if (!formula)
        goto cleanup;

    for (unsigned c = 0; c < formula->num_clauses; ++c)
    {
        skip_comments(fp);

        if(parse_clause(formula, fp, c))
            goto cleanup;
    }

cleanup:
    fclose(fp);
    return formula;
}
