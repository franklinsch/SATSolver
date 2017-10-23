#ifndef PARSER_H
#define PARSER_H

#include "formula.h"

/**
 Parse a formula in dimacs format contained in a text file

 @param path a relative path to the file containing the formula
 @return a malloced pointer to the resulting formula
*/
formula_t *parse_dimacs_file(char *path);

#endif // !PARSER_H
