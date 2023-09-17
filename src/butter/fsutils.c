/* fsutils.c ; Filesystem Utility functions
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

#include <butter/fsutils.h>

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <wordexp.h>

int copy_file(const char *to, const char *from) {
  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int saved_errno;

  fd_from = open(from, O_RDONLY);
  if (fd_from < 0)
    return -1;

  fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (fd_to < 0)
    goto out_error;

  while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0) {
        nread -= nwritten;
        out_ptr += nwritten;
      } else if (errno != EINTR) {
        goto out_error;
      }
    } while (nread > 0);
  }

  if (nread == 0) {
    if (close(fd_to) < 0) {
      fd_to = -1;
      goto out_error;
    }
    close(fd_from);

    /* Success! */
    return 0;
  }

out_error:
  saved_errno = errno;

  close(fd_from);
  if (fd_to >= 0)
    close(fd_to);

  errno = saved_errno;
  return -1;
}

char *expand_path(char *path) {
  wordexp_t expandres;
  wordexp(path, &expandres, 0);

  size_t toalloc = expandres.we_wordc;
  for (int i = 0; i < expandres.we_wordc; i++) {
    toalloc = strlen(expandres.we_wordv[i]);
  }

  char *expanded = malloc(toalloc + 1);
  size_t offset = 0;
  for (int i = 0; i < expandres.we_wordc; i++) {
    size_t wordsz = strlen(expandres.we_wordv[i]);
    memcpy(expanded + offset, expandres.we_wordv[i], wordsz);
    offset += wordsz;
    memcpy(expanded + offset, "/", 1);
    offset++;
  }

  expanded[toalloc] = 0;

  return expanded;
}
