#ifndef MB_UTILS_H
#define MB_UTILS_H

#define MB_LOGLVL_LOW 0
#define MB_LOGLVL_STD 1
#define MB_LOGLVL_IMP 2
#define MB_LOGLVL_SCR 3

extern int mb_logging_level;

void mb_log(int level, char *msg, const char *fmt, ...);

int str_startswith(char *str, char *start);
int str_endswith(char *str, char *end);

#endif
