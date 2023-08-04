/*
 * mariebuild: mb_script.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_SCRIPT_H
#define MB_SCRIPT_H

#include <mcfg.h>
#include <mariebuild/mb_types.h>

typedef struct mb_script {
  char      *name;
  char      *lines;
  mcfg_sector *parent_sector;
} mb_script_context;

int mb_exec_script(struct mb_script* script);

#endif
