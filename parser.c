#include "parser.h"

#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ignore_comments(FILE *fp)
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
    unsigned variables;
    unsigned clauses;

    ignore_comments(fp);

    fscanf(fp, "%c %s %u %u \n", &p, cnf, &variables, &clauses);

    if (p != 'p')
        return NULL;

    if (strncmp(cnf, "cnf", 3))
        return NULL;

    return alloc_formula(clauses, variables);
}

formula_t *parse_dimacs_file(char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        goto cleanup;

    formula_t *formula = parse_problem_line(fp);

    if (!formula)
        goto cleanup;

    for (unsigned c = 0; c < formula->clauses; ++c)
    {
        ignore_comments(fp);
        int *vars = calloc(formula->variables, sizeof (int));

        for (int v = 0; v < formula->variables; ++v)
        {
            int var;
            fscanf(fp, "%d ", &var);
            
            if (var == 0)
                break;

            if (abs(var) > formula->variables)
            {
                printf("Unknown variable: %d at clause %u\n", var, c);
                goto cleanup;
            }

            vars[v] = var < 0 ? var-- : var++;
        }

        add_clause(formula, vars);
    }

cleanup:
    fclose(fp);

    return formula;
}
