// strlist.c ; mariebuild string list implementationn
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "strlist.h"

#include <stdio.h>
#include <string.h>

#include "xmem.h"

strlist_t strlist_new(size_t capacity, bool heap_items) {
  strlist_t strlist = {
      .heap_items = heap_items,
      .capacity = capacity,
      .item_count = 0,
      .items = xmalloc(sizeof(char *) * (capacity + 1)),
  };

  return strlist;
}

void strlist_append(strlist_t *strlist, char *item) {
  if (strlist == NULL)
    return;

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
    return "";

  return strlist->items[index];
}

int strlist_contains_value(strlist_t *strlist, char *item) {
  if (strlist == NULL)
    return -1;

  for (size_t ix = 0; ix < strlist->item_count; ix++) {
    if (strlist->items[ix] != NULL && strcmp(strlist->items[ix], item) == 0)
      return ix;
  }

  return -1;
}

void strlist_destroy(strlist_t *strlist) {
  fprintf(stderr, "item count = %zu\n", strlist->item_count);
  if (strlist->heap_items) {
    for (size_t ix = 0; ix < strlist->item_count; ix++) {
      fprintf(stderr, "item %zu = %s\n", ix, strlist->items[ix]);
      xfree(strlist->items[ix]);

      for (size_t ix_2 = 0; ix_2 < strlist->item_count; ix_2++)
        if (strlist->items[ix] == strlist->items[ix_2])
          strlist->items[ix_2] = NULL;
    }
  }

  xfree(strlist->items);
}
