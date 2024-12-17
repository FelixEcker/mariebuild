// executor.h ; mariebuild script executor header
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef EXECUTOR_H
#define EXECUTOR_H

typedef struct process {
  int pid;

  char *location;
} process_t;

int mb_exec(char *script, char *name);

process_t mb_exec_parallel(char *script, char *name);

#endif // #ifndef EXECUTOR_H
