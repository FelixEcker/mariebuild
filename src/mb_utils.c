/*
 * mariebuild: mb_utils.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mariebuild/mb_utils.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>

#include <stdio.h>
#include <string.h>

int mb_logging_level = MB_LOGLVL_LOW;

void mb_logf(int level, char *msg, const char* fmt, ...) {
  if (level < mb_logging_level)
    return;

  char *level_prefix;
  switch (level) {
    case MB_LOGLVL_LOW: level_prefix = "l>"; break;
    case MB_LOGLVL_STD: level_prefix = ">>"; break;
    case MB_LOGLVL_IMP: level_prefix = "!!"; break;
    case MB_LOGLVL_SCR: level_prefix = "=>"; break;
    default: level_prefix = "_>"; break;
  }

  printf("%s ", level_prefix);
  printf(msg, fmt);
}

void mb_log(int level, char *msg) {
  mb_logf(level, msg, "");
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
    default:
      return "Unknown Errorcode";
  }
}

int str_startswith(char *str, char *start) {
  return 0;
}

int str_endswith(char *str, char *end) {
  int str_len = strlen(str);
  int end_len = strlen(end);

  return ((str_len > end_len) && !strcmp(str + str_len - end_len, end));
}
