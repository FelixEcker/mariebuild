// executor.c ; mariebuild script executor impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "executor.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

  int fd = open(name, O_RDWR | O_CREAT, 0777);
  FILE *outfile = fdopen(fd, "w");

  if (outfile == NULL) {
    mb_logf(LOG_ERROR, "failed to save script to \"%s\"\n", name);
    char *errname = strerror(errno);
    mb_logf(LOG_ERROR, "OS Error %d (%s)\n", errno, errname);
    xfree(errname);

    goto exit;
  }

  fprintf(outfile, "%s", script);

  fclose(outfile);
  close(fd);

  ret = system(name);
  if (ret != 0)
    mb_logf(LOG_ERROR, "execution for script \"%s\" failed: %d\n", name, ret);

  remove(name);

exit:
  xfree(name);
  return ret;
}
