/*
 * mariebuild: mb_execute.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mariebuild/mb_execute.h>

#include <mariebuild/mb_utils.h>

int mb_exec_script(struct mb_file* build_file, char *name, char *lines) {
  mb_logf(MB_LOGLVL_STD, "Executing script %s...\n", name);
  return MB_OK;
}

int mb_exec_build(struct mb_file build_file, struct mb_exec_params exec_params) {
  return MB_OK;
}
