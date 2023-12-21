/*
 * mariebuild: mb_utils.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mb_utils.h>

#include <mb_execute.h>

#include <butter/strutils.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mb_logging_level = MB_LOGLVL_LOW;
char *mb_errtext = NULL;

int mb_logf(int level, const char *format, ...) {
  if (level < mb_logging_level)
    return 0;

  char *level_prefix;
  switch (level) {
  case MB_LOGLVL_LOW:
    level_prefix = "l>";
    break;
  case MB_LOGLVL_STD:
    level_prefix = ">>";
    break;
  case MB_LOGLVL_IMP:
    level_prefix = "!!";
    break;
  case MB_LOGLVL_SCR:
    level_prefix = "=>";
    break;
  default:
    level_prefix = "_>";
    break;
  }

  fprintf(stderr, "%s ", level_prefix);

  va_list arg;
  int done;

  va_start(arg, format);
  done = vfprintf(stderr, format, arg);
  va_end(arg);

  return done;
}

void mb_log(int level, char *msg) { mb_logf(level, msg, ""); }

void *malloc_or_die(size_t size) {
  void *res = malloc(size);
  if (res == NULL)
    abort();
  return res;
}

char *get_build_errtext() {
  if (mb_errtext == NULL)
    return "";

  return mb_errtext;
}

char *errclass_msg(int err) {
  if ((err & MB_ERR_MASK_ERRNO) == MB_ERR_MASK_ERRNO)
    return "OS Error";

  if ((err & MB_BERR_MASK) == MB_BERR_MASK)
    return "Building Error";

  if ((err & MB_PERR_MASK) == MB_PERR_MASK)
    return "Parsing Error";

  return "Unknown Error-Class";
}

char *errcode_msg(int err) {
  switch (err) {
  case MB_ERR_UNKNOWN:
    return "Unknown Error";
  case MB_PERR_MISSING_REQUIRED:
    return "Missing required section";
  case MB_PERR_DUPLICATE_SECTION:
    return "Duplicated section";
  case MB_PERR_DUPLICATE_SECTOR:
    return "Duplicated sector";
  case MB_PERR_DUPLICATE_FIELD:
    return "Duplicated field";
  case MB_PERR_INVALID_IDENTIFIER:
    return "Invalid identifier";
  case MB_PERR_INVALID_SYNTAX:
    return "Invalid syntax";
  case MB_BERR_MISSING_FILES:
    return "Missing field .config/mariebuild/files";
  case MB_BERR_MISSING_COMPCMD:
    return "Missing field .confif/mariebuild/comp_cmd";
  case MB_BERR_SCRIPT_ERROR:
    return strcat("A script error occured:\n", get_build_errtext());
  case MB_BERR_COMPILE_ERROR:
    return "An error occured whilst compiling!";
  case MB_BERR_FINALIZE_ERROR:
    return "An errror occured whilst finalizing!";
  case MB_BERR_C_FILE_HANDLE_ERR:
    return "An invalid file handle was returned when trying to open a file!";
  case MB_BERR_C_FILE_WRITE_ERR:
    return "An error occured whilst writing data to a file!";
  default:
    if ((err & MB_ERR_MASK_ERRNO) == MB_ERR_MASK_ERRNO)
      return strerror(err & 0x0fffffff);
    return "Unknown Errorcode";
  }
}
