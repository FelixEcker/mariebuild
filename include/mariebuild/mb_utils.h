/*
 * mariebuild: mb_utils.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_UTILS_H
#define MB_UTILS_H

#ifdef _MSC_VER
#include <basetsd.h>
#include <stdio.h>
#include <stdlib.h>
typedef SSIZE_T ssize_t;
#endif

/******** Return Codes ********/

#define MB_OK          0x00000000
#define MB_ERR_UNKNOWN 0x00000001

/* Parsing Errors */

#define MB_PERR_MISSING_REQUIRED   0x00000101
#define MB_PERR_DUPLICATE_SECTION  0x00000102
#define MB_PERR_DUPLICATE_SECTOR   0x00000103
#define MB_PERR_DUPLICATE_FIELD    0x00000104
#define MB_PERR_INVALID_IDENTIFIER 0x00000105
#define MB_PERR_INVALID_SYNTAX     0x00000106

/* Build Errors */

#define MB_BERR_MISSING_FILES   0x00000201
#define MB_BERR_MISSING_COMPCMD 0x00000202
#define MB_BERR_SCRIPT_ERROR    0x00000203
#define MB_BERR_COMPILE_ERROR   0x00000204
#define MB_BERR_FINALIZE_ERROR  0x00000205

/* Mask if a system error occured which reports its error in errno */
#define MB_SERR_MASK_ERRNO       0x10000000

/******** Log Levels *********/

#define MB_LOGLVL_LOW 0
#define MB_LOGLVL_STD 1
#define MB_LOGLVL_IMP 2
#define MB_LOGLVL_SCR 3

/******** Globals ********/

extern const char *newline;
extern const char *str_terminator;

/* The minimum logging level to be output
 */
extern int mb_logging_level;

/* Additional error information
 */
extern char *mb_errtext;

/******** Platform Specific funcs ********/
/* Functions which are found within a POSIX C stdlib which are not present
 * under the microsoft c compiler are to be implemented here
 */

#ifdef _MSC_VER
size_t getline(char** lineptr, size_t* n, FILE* stream);
#endif

/******** Logging Functions ********/

int mb_logf(int level, const char *format, ...);
void mb_log(int level, char *msg);

/******** Misc. Functions ********/

/* Translate an error code to an error message
 */
char *errcode_msg(int err);

int str_startswith(char *str, char *start);
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

#endif
