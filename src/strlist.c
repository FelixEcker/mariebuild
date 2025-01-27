/* strlist.c ; mariebuild string list implementationn
 *
 * Copyright (c) 2024, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#include <stdio.h>
#include <string.h>

#include "strlist.h"
#include "xmem.h"

strlist_t strlist_new(size_t capacity, bool heap_items) {
	strlist_t strlist = {
		.heap_items = heap_items,
		.capacity = capacity,
		.item_count = 0,
		.items = XMALLOC(sizeof(char *) * (capacity + 1)),
	};

	return strlist;
}

void strlist_append(strlist_t *strlist, char *item) {
	if (strlist == NULL) {
		return;
	}

	if (strlist->item_count >= strlist->capacity) {
		strlist->capacity = sizeof(*strlist->items) * strlist->capacity * 2;
		strlist->items = XREALLOC(
			strlist->items, strlist->capacity);
	}

	if(strlist->items == NULL) {
		strlist->items = XMALLOC(32 * sizeof(*strlist->items));
		strlist->capacity = 32;
		strlist->item_count = 0;
	}

	strlist->items[strlist->item_count] = item;
	strlist->item_count++;
}

char *strlist_get(strlist_t *strlist, size_t index) {
	if (index >= strlist->item_count) {
		return "";
	}

	return strlist->items[index];
}

int strlist_contains_value(strlist_t *strlist, char *item) {
	if (strlist == NULL) {
		return -1;
	}

	for (size_t ix = 0; ix < strlist->item_count; ix++) {
		if (strlist->items[ix] != NULL &&
			strcmp(strlist->items[ix], item) == 0) {
			return ix;
		}
	}

	return -1;
}

void strlist_destroy(strlist_t *strlist) {
	if (strlist == NULL || strlist->items == NULL) {
		return;
	}

	if (strlist->heap_items) {
		for (size_t ix = 0; ix < strlist->item_count; ix++) {
			XFREE(strlist->items[ix]);

			for (size_t ix_2 = 0; ix_2 < strlist->item_count; ix_2++) {
				if (strlist->items[ix] == strlist->items[ix_2]) {
					strlist->items[ix_2] = NULL;
				}
			}
		}
	}

	XFREE(strlist->items);
}
