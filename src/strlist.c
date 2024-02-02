// strlist.c ; mariebuild string list implementationn
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "strlist.h"

#include "xmem.h"

strlist_t strlist_new(size_t capacity) {
  strlist_t strlist = {
      .capacity = capacity,
      .item_count = 0,
      .items = xmalloc(capacity),
  };

  return strlist;
}

void strlist_append(strlist_t *strlist, char *item) {
  size_t ix = strlist->item_count;
  strlist->item_count++;

  if (strlist->item_count >= strlist->capacity) {
    strlist->items =
        xrealloc(strlist->items, sizeof(*strlist->items) * strlist->item_count);
  }

  strlist->items[ix] = item;
}

char *strlist_get(strlist_t *strlist, size_t index) {
  if (index >= strlist->item_count)
    return NULL;

  return strlist->items[index];
}

void strlist_destroy(strlist_t *strlist) { xfree(strlist->items); }
