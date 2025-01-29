/* build.h ; mariebuild build functionality header
 *
 * Copyright (c) 2024, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#ifndef BUILD_H
#define BUILD_H

#include "logging.h"
#include "mcfg.h"
#include "types.h"

typedef struct args {
	char *buildfile;
	char *target;
	bool force;
	bool no_splash;
	bool keep_going; /* they're hot on your heels! */
	log_level_t verbosity;
	bool verbosity_overriden; /* helper flag for verbosity */
} args_t;

int mb_start(args_t args);

int mb_begin_build(mcfg_file_t *file, config_t cfg);

#endif /* #ifndef BUILD_H */
