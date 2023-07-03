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

/* Commonly used Fieldname constants */
char fn_files[] = ".config/mariebuild/files";
char fn_comp_cmd[] = ".config/mariebuild/comp_cmd";

int check_required_fields(struct mb_file* file) {
  mb_field *f_files = find_field(file, fn_files);
  if ((f_files == NULL) || (f_files->value == NULL))
    return MB_BERR_MISSING_FILES;
  
  mb_field *f_comp_cmd = find_field(file, fn_comp_cmd);

  if ((f_comp_cmd == NULL) || (f_comp_cmd->value == NULL))
    return MB_BERR_MISSING_COMPCMD;
  
  return MB_OK;
}

int mb_exec_script(struct mb_build* build, char *name, char *lines) {
  mb_logf(MB_LOGLVL_STD, "Executing script %s...\n", name);
  return MB_OK;
}

/******** Build Stage Functions ********/

int mb_exec_prepare(struct mb_build* build) {
  mb_section *prepare_script = find_section(build->build_file, 
                                            ".scripts/prepare");
  if (prepare_script == NULL)
    goto prepare_finished;

  if (prepare_script->lines == NULL)
    goto prepare_finished;

  mb_exec_script(build, "prepare", prepare_script->lines);

prepare_finished:
  return MB_OK;
}

int mb_exec_prepare_mode(struct mb_build* build, char *mode) {
  return MB_OK;
}

int mb_exec_compile(struct mb_build* build) {
  int result = check_required_fields(build->build_file);
  if (result != MB_OK)
    return result;

  // Ready requried data from build->build_file
  // File list
  mb_field *f_files = find_field(build->build_file, fn_files);
  char *files_cpy = malloc(strlen(f_files->value)+1);
  strcpy(files_cpy, f_files->value);
  
  mb_field *f_comp_cmd = find_field(build->build_file, fn_comp_cmd);

  mb_logf(MB_LOGLVL_LOW, "File List: %s\n", f_files->value);
  char delim[] = ":";
  char *file = strtok(files_cpy, delim);

  char pfile_field[] = ".config/mariebuild/file";
  mb_field *f_file = find_field(build->build_file, pfile_field);
  
  // register field file if not existing
  char pmariebuild[] = ".config/mariebuild";
  if (f_file == NULL) {
    register_field(find_section(build->build_file, pmariebuild), "file", "");
    f_file = find_field(build->build_file, pfile_field);
  }

  // Register field mode_flags if not existing
  char pmodeflags_field[] = ".config/mariebuild/mode_flags";
  mb_field *f_modeflags = find_field(build->build_file, pmodeflags_field);
  if (f_modeflags == NULL) {
    register_field(
        find_section(build->build_file, pmariebuild)
      , "mode_flags"
      , ""
    );

    // Reassing previously assigned fields because the original pointer gets
    // broken by new field being registered.
    f_file = find_field(build->build_file, pfile_field);
    f_comp_cmd = find_field(build->build_file, fn_comp_cmd);
  }

  // Run compilation command for each file
  while (file != NULL) {
    if (f_file->value != NULL) free(f_file->value);
    f_file->value = malloc(strlen(file)+1);
    strcpy(f_file->value, file);

    char *cmd = resolve_fields((*build->build_file), f_comp_cmd->value, 
                               ".config/mariebuild/");
    mb_logf(MB_LOGLVL_STD, "%s\n", cmd);
    file = strtok(NULL, delim);

    int retc = system(cmd);
    free(cmd);

    // Handle errored command
    if (retc != 0) {
      retc = retc / 256;
      mb_logf(MB_LOGLVL_IMP, 
              "Compilation Command returned non-zero exit code: %d\n", retc);

      free(files_cpy);
      return MB_BERR_COMPILE_ERROR;
    }
  }

  free(files_cpy);
  return MB_OK;
}

int mb_exec_finalize(struct mb_build* build) {
  int result = check_required_fields(build->build_file);
  if (result != MB_OK)
    return result;

  char pfinalize_cmd[] = ".config/mariebuild/finalize_cmd";
  mb_field *f_finalize_cmd = find_field(build->build_file, pfinalize_cmd);
  if ((f_finalize_cmd == NULL) || (f_finalize_cmd->value == NULL))
    return MB_OK;

  char *cmd = resolve_fields((*build->build_file), f_finalize_cmd->value,
                             ".config/mariebuild/");
  mb_logf(MB_LOGLVL_STD, "%s\n", cmd);
  
  int retc = system(cmd);
  if (retc != 0) {
    retc = retc / 256;
    mb_logf(MB_LOGLVL_IMP, 
            "Finalization Command returned non-zero exit code: %d\n", retc);
    result = MB_BERR_FINALIZE_ERROR;
  }

  free(cmd);
  return result;
}

int mb_exec_build(struct mb_file* build_file, 
                  struct mb_exec_params exec_params) {
  mb_build build;
  build.stage = MB_STAGE_NONE;
  build.build_file = build_file;

  int result;

  mb_log(MB_LOGLVL_LOW, "Entering prepration stage\n");
  result = mb_exec_prepare(&build);
  if (result != MB_OK)
    return result;

  mb_logf(MB_LOGLVL_LOW, "Entering %s-preparation stage\n", exec_params.mode);
  result = mb_exec_prepare_mode(&build, exec_params.mode);
  if (result != MB_OK)
    return result;

  mb_log(MB_LOGLVL_LOW, "Entering compilation stage\n");
  result = mb_exec_compile(&build);
  if (result != MB_OK) 
    return result;

  mb_log(MB_LOGLVL_LOW, "Finishing up...\n");
  result = mb_exec_finalize(&build);
  if (result != MB_OK)
    return result;

  mb_exec_script(&build, "test", "test");

  return MB_OK;
}
