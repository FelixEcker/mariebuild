#include <mariebuild/mb_utils.h>

#include <stdio.h>
#include <string.h>

int mb_logging_level = MB_LOGLVL_LOW;

void mb_log(int level, char *msg, const char* fmt, ...) {
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

int str_startswith(char *str, char *start) {
  return 0;
}

int str_endswith(char *str, char *end) {
  int str_len = strlen(str);
  int end_len = strlen(end);

  return ((str_len > end_len) && !strcmp(str + str_len - end_len, end));
}
