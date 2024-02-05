// types.h ; mariebuild shared typedefinition header
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef TYPES_H
#define TYPES_H

typedef struct config {
  char *default_target;
  char *target;
  strlist_t public_targets;
  bool always_force;
  bool ignore_failures;
} config_t;

#endif // #ifndef TYPES_H
