#include "dpll.h"

#include "clause.h"
#include "formula.h"
#include "implication_graph.h"

#include <stdlib.h>
#include <stdbool.h>


bool dpll(formula_t *formula)
{
 /*
    node_t root;
    node_init(&root, formula, 0);

    node_t *curr = &root;

    clause_t *end = formula->clauses + formula->num_clauses;

    for (clause_t *clause = formula->clauses; clause != end; clause++)
    {
        for (int i = 0; i < clause->size; i++)
        {
            int variable = clause_get_var(clause, i);
            int assignment = find_assignment(curr, variable);
            if (assignment == ASSIGNMENT_NOT_FOUND)
            {
                node_t *new_assignment = malloc(sizeof (node_t));
                node_init(new_assignment, formula, curr->depth + 1);
                new_assignment->assignments[0] = variable;
                node_add_parent(new_assignment, curr);
                node_add_children(curr, new_assignment);

                curr = new_assignment;
            }
            else if (assignment == variable)
            {
                continue;
            }
            else
            {
                node_t *parent = curr->parents[0];
                delete_node(curr);
                curr = parent;
            }
        }
    }

    // SAT

    bool all_assigned = true;

    for (int i = 1; i <= formula->num_variables; i++)
    {
        all_assigned &= is_assigned(&root, i);
    }

break_clause:
    node_free(&root);
    return all_assigned;
//*/
    return false;
}
