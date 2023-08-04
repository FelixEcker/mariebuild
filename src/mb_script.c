/*
 * mariebuild: mb_script.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mariebuild/mb_script.h>

#include <mariebuild/mb_utils.h>

int mb_exec_script(struct mb_script* script) {
  mb_logf(MB_LOGLVL_STD, "Executing script %s...\n", script->name);
  return MB_OK;
}
