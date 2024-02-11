// mcfg_shared.h ; marie config format internal shared function header
// for MCFG/2
//
// Copyright (c) 2023, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef MCFG_SHARED_H
#define MCFG_SHARED_H

#include <stdbool.h>
#include <sys/types.h>

void *malloc_or_die(size_t size);
void *realloc_or_die(void *org, size_t size);

bool is_string_empty(char *in);

char *remove_newline(char *in);

bool has_newline(char *in);

#endif
