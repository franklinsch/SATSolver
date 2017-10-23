#ifndef PARSER_H
#define PARSER_H

#include "formula.h"

/**
 Parse a formula in DIMACS format contained in the text file at the given path.

 @param path A relative path to the file containing the formula.
 @return A pointer to the resulting formula.
*/
formula_t *parse_dimacs_file(char *path);

#endif // !PARSER_H
