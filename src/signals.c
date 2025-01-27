/* signals.c ; mariebuild build signal handling
 *
 * Copyright (c) 2025, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "signals.h"
#include "strlist.h"

strlist_t tmp_files;
bool initialised = false;

void mb_signal_generic_handler(int signal) {
	mb_logf(LOG_ERROR, "signal %d received, quitting...\n", signal);
	for (size_t ix = 0; ix < tmp_files.item_count; ix++) {
		char *item = strlist_get(&tmp_files, ix);
		if (item == NULL) {
			continue;
		}

		remove(item);
	}

	exit(-1);
}

void mb_install_signal_handlers(void) {
	signal(SIGHUP, &mb_signal_generic_handler);
	signal(SIGINT, &mb_signal_generic_handler);
	signal(SIGQUIT, &mb_signal_generic_handler);
	signal(SIGTERM, &mb_signal_generic_handler);

	tmp_files = strlist_new(16, true);
	initialised = true;
}

void mb_register_tmp_file(char *path) {
	if (!initialised) {
		return;
	}

	strlist_append(&tmp_files, strdup(path));
}
