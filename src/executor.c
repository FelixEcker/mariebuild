// executor.c ; mariebuild script executor impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "executor.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "logging.h"
#include "xmem.h"

bool has_shebang(char *script) {
  const char *shebang = "#!";
  return strncmp(shebang, script, strlen(shebang));
}

char *create_name(char *name) {
  pid_t pid = getpid();

  const char *prefix = "/tmp/";

  size_t pid_size = (size_t)floor(log10((double)INT_MAX));
  size_t size = sizeof(prefix) + pid_size + strlen(name) + 1;

  char *ret = xmalloc(size);
  snprintf(ret, size, "%s%d.%s", prefix, pid, name);

  return ret;
}

int mb_exec(char *script, char *name) {
  int ret = 0;

  name = create_name(name);
  mb_logf(LOG_DEBUG, "writing script to \"%s\"\n", name);
  FILE *outfile = fopen(name, "w");

  fprintf(outfile, "%s", script);

  fclose(outfile);

  xfree(name);
  return ret;
}
