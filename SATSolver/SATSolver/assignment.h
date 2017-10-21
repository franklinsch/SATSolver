//
//  assignment.h
//  SATSolver
//
//  Created by Franklin Schrans on 10/21/17.
//  Copyright © 2017 Franklin Schrans. All rights reserved.
//

#ifndef assignment_h
#define assignment_h

#include <stdbool.h>

// A BCP assignment made to a variable.
typedef struct {
  int variable;
  
  // The target value of literal after the assignment.
  bool value;
  
  // The depth at which we assigned.
  int depth;
} assignment_t;

#endif /* assignment_h */
