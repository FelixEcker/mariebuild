/*
 * mariebuild: mb_execute.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <mariebuild/mb_execute.h>

#include <mariebuild/mb_utils.h>
#include <mariebuild/mb_parse.h>

#include <stdlib.h>
#include <stdio.h>
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
  char pfile[] = ".config/mariebuild/files";
  mb_field *f_files = find_field(build->build_file, pfile);

  if ((f_files == NULL) || (f_files->value == NULL))
    return MB_BERR_MISSING_FILES;

  char *files_cpy = malloc(strlen(f_files->value)+1);
  strcpy(files_cpy, f_files->value);
  
  char pcomp_cmd[] = ".config/mariebuild/comp_cmd";
  mb_field *f_comp_cmd = find_field(build->build_file, pcomp_cmd);

  if ((f_comp_cmd == NULL) || (f_comp_cmd->value == NULL))
    return MB_BERR_MISSING_COMPCMD;

  mb_logf(MB_LOGLVL_LOW, "File List: %s\n", f_files->value);
  char delim[] = ":";
  char *file = strtok(files_cpy, delim);

  char pfile_field[] = ".config/mariebuild/file";
  mb_field *f_file = find_field(build->build_file, pfile_field);
  
  if (f_file == NULL) {
    char pmariebuild[] = ".config/mariebuild";
    register_field(find_section(build->build_file, pmariebuild), "file", "");
    f_file = find_field(build->build_file, pfile_field);
  }

  while (file != NULL) {
    if (f_file->value != NULL) free(f_file->value);
    f_file->value = malloc(strlen(file)+1);
    strcpy(f_file->value, file);

    char *cmd = resolve_fields((*build->build_file), f_comp_cmd->value, 
                               ".config/mariebuild/");
    mb_logf(MB_LOGLVL_STD, "%s\n", cmd);
    file = strtok(NULL, delim);


    free(cmd);
  }

  free(files_cpy);
  return MB_OK;
}

int mb_exec_finalize(struct mb_build* build) {
  return MB_OK;
}

int mb_exec_build(struct mb_file* build_file, 
                  struct mb_exec_params exec_params) {
  mb_build build;
  build.stage = MB_STAGE_NONE;
  build.build_file = build_file;

  int result;

  result = mb_exec_compile(&build);
  if (result != MB_OK) 
    return result;

  mb_exec_script(&build, "test", "test");

  return MB_OK;
}
