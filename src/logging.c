#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "ansi.h"

log_level_t mb_log_level;

log_level_t str_to_loglvl(char *str) {
  if (str == NULL)
    return LOG_DEBUG;

  long int converted = strtol(str, NULL, 10);

  switch (converted) {
  case LOG_DEBUG:
    return LOG_DEBUG;
  case LOG_INFO:
    return LOG_INFO;
  case LOG_WARNING:
    return LOG_WARNING;
  case LOG_ERROR:
    return LOG_ERROR;
  default:
    return LOG_INVALID;
  }
}

int mb_logf(log_level_t level, const char *format, ...) {
  if (level < mb_log_level)
    return 0;

  char *level_prefix;
  switch (level) {
  default:
  case LOG_DEBUG:
    level_prefix = "---";
    break;
  case LOG_INFO:
    level_prefix = ANSI_BOLD ANSI_FG_GREEN "==>" ANSI_RESET;
    break;
  case LOG_WARNING:
    level_prefix = ANSI_BOLD ANSI_FG_YELLOW "WRN" ANSI_RESET;
    break;
  case LOG_ERROR:
    level_prefix = ANSI_BOLD ANSI_FG_RED "ERR" ANSI_RESET;
    break;
  }

  fprintf(stderr, "%s %s", level_prefix, ANSI_BOLD);

  va_list arg;
  int done;

  va_start(arg, format);
  done = vfprintf(stderr, format, arg);
  va_end(arg);

  fprintf(stderr, ANSI_RESET);

  return done;
}

int mb_logf_noprefix(log_level_t level, const char *format, ...) {
  if (level < mb_log_level)
    return 0;

  va_list arg;
  int done;

  va_start(arg, format);
  done = vfprintf(stderr, format, arg);
  va_end(arg);

  return done;
}

void mb_log(int level, char *msg) { mb_logf(level, msg, ""); }
