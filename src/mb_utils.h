/*
 * mariebuild: mb_utils.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_UTILS_H
#define MB_UTILS_H

#include <stdlib.h>

#include <mcfg.h>

/******** Return Codes ********/

#define MB_OK 0x00000000
#define MB_ERR_UNKNOWN 0x00000001

/* Parsing Errors */

#define MB_PERR_MASK MCFG_PERR_MASK
#define MB_PERR_MISSING_REQUIRED MCFG_PERR_MISSING_REQUIRED
#define MB_PERR_DUPLICATE_SECTION MCFG_PERR_DUPLICATE_SECTION
#define MB_PERR_DUPLICATE_SECTOR MCFG_PERR_DUPLICATE_SECTOR
#define MB_PERR_DUPLICATE_FIELD MCFG_PERR_DUPLICATE_FIELD
#define MB_PERR_INVALID_IDENTIFIER MCFG_PERR_INVALID_IDENTIFIER
#define MB_PERR_INVALID_SYNTAX MCFG_PERR_INVALID_SYNTAX

/* Build Errors */

#define MB_BERR_MASK 0x20000000
#define MB_BERR_MISSING_FILES 0x20000001
#define MB_BERR_MISSING_COMPCMD 0x20000002
#define MB_BERR_SCRIPT_ERROR 0x20000003
#define MB_BERR_COMPILE_ERROR 0x20000004
#define MB_BERR_FINALIZE_ERROR 0x20000005
#define MB_BERR_C_FILE_HANDLE_ERR 0x20000006
#define MB_BERR_C_FILE_WRITE_ERR 0x20000007

/* Mask if a system error occured which reports its error in errno */
#define MB_ERR_MASK_ERRNO 0xf0000000

/******** Log Levels *********/

#define MB_LOGLVL_LOW 0
#define MB_LOGLVL_STD 1
#define MB_LOGLVL_IMP 2
#define MB_LOGLVL_SCR 3

/******** Globals ********/

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

/******** Logging Functions ********/

int mb_logf(int level, const char *format, ...);
void mb_log(int level, char *msg);

/******** Misc. Functions ********/

/* Allocate or die
 */
void *malloc_or_die(size_t size);

char *get_build_errtext();

/* Get the name of the given error codes class
 */
char *errclass_msg(int err);

/* Translate an error code to an error message
 */
char *errcode_msg(int err);

#endif
