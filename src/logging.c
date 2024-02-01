#include "logging.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

log_level_t mb_log_level;

log_level_t str_to_loglvl(char *str) {
  long int converted = strtol(str, NULL, 10);
  
  switch (converted) {
  case LOG_DEBUG: return LOG_DEBUG;
  case LOG_INFO: return LOG_INFO;
  case LOG_WARNING: return LOG_WARNING;
  case LOG_ERROR: return LOG_ERROR;
  default: return LOG_INVALID;
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
    level_prefix = "==>";
    break;
  case LOG_WARNING:
    level_prefix = "WRN";
    break;
  case LOG_ERROR:
    level_prefix = "ERR";
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
