/* strutils.h ; String Utility functions
 * Butter Utiltiy Library
 */

/* Licensed under the WTFPL version 2
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *  
 * Copyright (C) 2023, Marie Eckert
 * Copyright (C) 2010-2022, ThhE <thhe@gmx.de>
 * 
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *  
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 * 
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/* This library is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. 
 */

#ifndef BUTTER_STRUTILS_H
#define BUTTER_STRUTILS_H

extern const char str_terminator[];
extern const char newline[];

/* Returns true (1) if string "str" starts with the string "start"
 */
int str_startswith(char *str, char *start);

/* Returns true (1) if stirng "str" ends with the string "end"
 */
int str_endswith(char *str, char *end);

/* Creates a stringcopy from string src until the first occurence of
 * the delimiter char is hit
 */
char *strcpy_until(char *src, char delimiter);

/* Creates a stringcopy from string src until the first occurence of
 * the delimiter char is hit before the start of src.
 *
 * E.g. delimiter = "."
 *      input = "foo.bar"
 *      pointer        ^
 *
 * searches backwards until "." is hit, the it creates a string copy from
 * the pointer of "." to the pointer of src:
 *      result = strcpy("foor.bar")
 *                           ^~~^
 *
 * src_org : Pointer of the origin of src, so that the search does not
 *           underrun.
 */
char *bstrcpy_until(char *src, char *src_org, char delimiter);

/* Replaces every occurence of string "substr" in string "src" with
 * string "new"
 */
char *str_replace(char *orig, char *rep, char *with);

/* Strips leading and trailing whitespace from the input string.
 *
 * Note: This function returns a pointer to a substring of the original string.
 * If the given string was allocated dynamically, the caller must not overwrite
 * that pointer with the returned value, since the original pointer must be
 * deallocated using the same allocator with which it was allocated.
 * The return value must NOT be deallocated using free() etc.
 * */
char *trim_whitespace(char *str);

#endif
