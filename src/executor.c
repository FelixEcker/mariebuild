/* executor.c ; mariebuild script executor impl.
 *
 * Copyright (c) 2024, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 2

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "executor.h"
#include "logging.h"
#include "xmem.h"

/* this is such a disgusting hack i dont even want to think about it */
static uint64_t script_counter = 0;

bool has_shebang(char *script) {
	const char *shebang = "#!";
	return strncmp(shebang, script, strlen(shebang));
}

char *create_name_frompid(char *name, pid_t pid) {
	const char *prefix = "/tmp/";

	size_t pid_size = (size_t)floor(log10((double)INT_MAX));
	size_t size = sizeof(prefix) + pid_size + strlen(name) + 1;

	char *ret = XMALLOC(size);
	snprintf(ret, size, "%s%d_%lu.%s", prefix, pid, script_counter, name);
	script_counter++;

	return ret;
}

char *create_name(char *name) {
	return create_name_frompid(name, getpid());
}

int _prepare_exec(char *script, char **name) {
	*name = create_name(*name);
	mb_logf(LOG_DEBUG, "writing script to \"%s\"\n", *name);

	int fd = open(*name, O_RDWR | O_CREAT, 0777);
	FILE *outfile = fdopen(fd, "w");

	if (outfile == NULL) {
		mb_logf(LOG_ERROR, "failed to save script to \"%s\"\n", *name);
		char *errname = strerror(errno);
		mb_logf(LOG_ERROR, "OS Error %d (%s)\n", errno, errname);
		XFREE(errname);

		return 1;
	}

	fprintf(outfile, "%s", script);

	fclose(outfile);
	close(fd);

	return 0;
}

int mb_exec(char *script, char *name) {
	int ret = _prepare_exec(script, &name);
	if (ret != 0) {
		goto exit;
	}

	ret = system(name);
	if (ret != 0) {
		mb_logf(
			LOG_ERROR, "execution for script \"%s\" failed: 0x%08x%s\n", name,
			ret, ret < 0 ? " (system() call failed)" : "");
	}

	remove(name);

exit:
	XFREE(name);
	return WEXITSTATUS(ret);
}

process_t mb_exec_parallel(char *script, char *name) {
	int ret = _prepare_exec(script, &name);
	if (ret != 0) {
		return (process_t){.pid = 0, .location = NULL};
	}

	int pid = fork();
	if (pid != 0) {
		return (process_t){.pid = pid, .location = name};
	}

	execl("/bin/sh", "sh", "-c", name, (char *)NULL);
	__builtin_unreachable();
}
