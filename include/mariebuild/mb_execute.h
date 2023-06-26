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

#define MB_STAGE_NONE        -1
#define MB_STAGE_PREPARE      0
#define MB_STAGE_PREPARE_MODE 1
#define MB_STAGE_COMPILE      2
#define MB_STAGE_FINALIZE     3

/* Holds execution parameters for a build
 */
typedef struct mb_exec_params {
  char *exec_script;
  char *platform;
  int  *force;
  int  *status;
} mb_exec_params;

/* Used to hold data of current build 
 */
typedef struct mb_build {
  int             stage;
  struct mb_file* build_file;
} mb_build;

/* Executes a script section.
 * name  : The name of the script being executed
 * lines : All lines of the script being run
 */
int mb_exec_script(struct mb_build* build, char *name, char *lines);

/******** Build Stage functions ********/
/* See docs/build_stages.md for details on the build stages */

int mb_exec_prepare(struct mb_build* build);
int mb_exec_prepare_mode(struct mb_build* build);
int mb_exec_compile(struct mb_build* build);
int mb_exec_finalize(struct mb_build* build);

/* Execute an entire build for the given build file
 */
int mb_exec_build(struct mb_file* build_file, 
                  struct mb_exec_params exec_params);

#endif
