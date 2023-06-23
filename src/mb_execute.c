/*
 * mariebuild: mb_execute.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mariebuild/mb_execute.h>

#include <mariebuild/mb_utils.h>
#include <mariebuild/mb_parse.h>

#include <stdlib.h>
#include <string.h>

/******** mb_execute.h ********/

int mb_exec_script(struct mb_build* build, char *name, char *lines) {
  mb_logf(MB_LOGLVL_STD, "Executing script %s...\n", name);
  return MB_OK;
}

/******** Build Stage Functiobs ********/

int mb_exec_prepare(struct mb_build* build) {
  return MB_OK;
}

int mb_exec_prepare_mode(struct mb_build* build) {
  return MB_OK;
}

int mb_exec_compile(struct mb_build* build) {
  char bfile[] = ".config/mariebuild/files";
  mb_field *f_files = find_field(&build->build_file, 
                                 bfile);

  if ((f_files == NULL) || (f_files->value == NULL))
    return MB_BERR_MISSING_FILES;

  return MB_OK;
}

int mb_exec_finalize(struct mb_build* build) {
  return MB_OK;
}

int mb_exec_build(struct mb_file build_file, 
                  struct mb_exec_params exec_params) {
  mb_build build;
  build.file = NULL;
  build.stage = MB_STAGE_NONE;
  build.build_file = build_file;

  int result;

  result = mb_exec_compile(&build);
  if (result != MB_OK) 
    return result;

  mb_exec_script(&build, "test", "test");

  return MB_OK;
}
