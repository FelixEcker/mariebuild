#include <mariebuild/mb_utils.h>

#include <string.h>

int str_startswith(char *str, char *start) {
  return 0;
}

int str_endswith(char *str, char *end) {
  int result = 0;
  int str_len = strlen(str);
  int end_len = strlen(end);

  return ((str_len > end_len) && !strcmp(str + str_len - end_len, end));
}
