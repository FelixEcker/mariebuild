#include <butter/strutils.h>

#include <stdlib.h>
#include <string.h>

const char str_terminator[] = "\0";

int str_startswith(char *str, char *start) {
  return strncmp(start, str, strlen(start));
}

int str_endswith(char *str, char *end) {
  int str_len = strlen(str);
  int end_len = strlen(end);

  return ((str_len > end_len) && !strcmp(str + str_len - end_len, end));
}

char *strcpy_until(char *src, char delimiter) {
  int offs = 0;
  while (offs < strlen(src)) {
    if (src[offs] == delimiter) break;
    offs++;
  }

  if (offs == 0)
    return "";

  char *res = malloc(offs+1);
  memcpy(res, src, offs);
  memcpy(res+offs, str_terminator, 1);
  
  return res;
}

char *bstrcpy_until(char *src, char *src_org, char delimiter) {
  int offs = 0;
  while ((src-offs) > src_org) {
    if ((src-offs)[0] == delimiter) break;
    offs++;
  }

  if (offs == 0)
    return "";

  char *res = malloc(offs+1);
  memcpy(res, src-offs+1, offs);
  memcpy(res+offs, str_terminator, 1);
  
  return res;
}

char *str_replace(char *orig, char *rep, char *with) {
  char *result; // the return string
  char *ins;    // the next insert point
  char *tmp;    // varies
  int len_rep;  // length of rep (the string to remove)
  int len_with; // length of with (the string to replace rep with)
  int len_front; // distance between rep and end of last rep
  int count;    // number of replacements

  // sanity checks and initialization
  if (!orig || !rep)
    return NULL;
  len_rep = strlen(rep);
  if (len_rep == 0)
    return NULL; // empty rep causes infinite loop during count
  if (!with)
    with = "";
  len_with = strlen(with);

  // count the number of replacements needed
  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    return NULL;

  // first time through the loop, all the variable are set correctly
  // from here on,
  //    tmp points to the end of the result string
  //    ins points to the next occurrence of rep in orig
  //    orig points to the remainder of orig after "end of rep"
  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
  }

  strcpy(tmp, orig);
  return result;
}

