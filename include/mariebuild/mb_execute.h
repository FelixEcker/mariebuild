/*
 * mariebuild: mb_execute.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_EXECUTE_H
#define MB_EXECUTE_H

#include <mariebuild/mb_parse.h>

typedef struct mb_exec_params {
  char *exec_script;
  char *platform;
  int  *force;
  int  *status;
} mb_exec_params;

int mb_exec_script(struct mb_file* build_file, char *name, char *lines);
int mb_exec_build(struct mb_file build_file, 
                  struct mb_exec_params exec_params);

#endif
