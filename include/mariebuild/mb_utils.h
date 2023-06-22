/*
 * mariebuild: mb_utils.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_UTILS_H
#define MB_UTILS_H

#define MB_LOGLVL_LOW 0
#define MB_LOGLVL_STD 1
#define MB_LOGLVL_IMP 2
#define MB_LOGLVL_SCR 3

extern int mb_logging_level;

void mb_log(int level, char *msg, const char *fmt, ...);
char *errcode_msg(int err);

int str_startswith(char *str, char *start);
int str_endswith(char *str, char *end);

#endif
