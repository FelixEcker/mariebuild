/*
 * mariebuild: mb_utils.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <mariebuild/mb_utils.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

const char *newline = "\n";

int mb_logging_level = MB_LOGLVL_LOW;
char *mb_errtext = "";

#ifdef _MSC_VER

/* This code is public domain -- Will Hartung 4/9/09 */
size_t getline(char** lineptr, size_t* n, FILE* stream) {
    char* bufptr = NULL;
    char* p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

int mb_logf(int level, const char *format, ...) {
  if (level < mb_logging_level)
    return 0;

  char *level_prefix;
  switch (level) {
    case MB_LOGLVL_LOW: level_prefix = "l>"; break;
    case MB_LOGLVL_STD: level_prefix = ">>"; break;
    case MB_LOGLVL_IMP: level_prefix = "!!"; break;
    case MB_LOGLVL_SCR: level_prefix = "=>"; break;
    default: level_prefix = "_>"; break;
  }

  printf("%s ", level_prefix);

  va_list arg;
  int done;

  va_start(arg, format);
  done = vfprintf(stdout, format, arg);
  va_end(arg);

  return done;
}

void mb_log(int level, char *msg) {
  mb_logf(level, msg, "");
}

char *get_build_errtext() {
  if (mb_errtext == NULL)
    return "";

  return mb_errtext;
}

char *errclass_msg(int err) {
  if ((err & MB_PERR_MASK) == 0)
    return "Parsing Error";

  if ((err & MB_BERR_MASK) == 0)
    return "Building Error";

  if ((err & MB_SERR_MASK) == 0)
    return "Script Error";

  if ((err & MB_ERR_MASK_ERRNO) == 0)
    return "System Error";

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
    case MB_SERR_INVALID_SYNTAX:
      return "Invalid syntax";
    case MB_SERR_INVALID_IDENTIFIER:
      return "Invalid identifier";
    case MB_SERR_SCRIPT_NOT_FOUND:
      return "Script not found";
    default:
      return "Unknown Errorcode";
  }
}

