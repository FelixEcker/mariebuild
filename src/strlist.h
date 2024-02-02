// strlist.h ; mariebuild string list header
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef STRLIST_H
#define STRLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct strlist {
  size_t capacity;
  size_t item_count;
  char **items;
} strlist_t;

strlist_t strlist_new(size_t capacity);

void strlist_append(strlist_t *strlist, char *item);

char *strlist_get(strlist_t *strlist, size_t index);

int strlist_contains_value(strlist_t *strlist, char *item);

void strlist_destroy(strlist_t *strlist);

#endif // #infdef STRLIST_H
