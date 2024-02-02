// xmem.c ; mariebuild memory helpers impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "xmem.h"

#if XMEM_LOGGING
#include <stdio.h>
#endif

#include <stdlib.h>

void *xmalloc(size_t size) {
  void *ret = malloc(size);
  if (ret == NULL) {
#if XMEM_LOGGING
    fprintf(stderr, "xmalloc failed, size = %zu", size);
#endif
    abort();
  }

  return ret;
}

void *xrealloc(void *org, size_t size) {
  void *ret = realloc(org, size);
  if (ret == NULL) {
#if XMEM_LOGGING
    fprintf(stderr, "xrealloc failed, org = %p ; size = %zu", org, size);
#endif
    abort();
  }

  return ret;
}

void xfree(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
    return;
  }

#if XMEM_LOGGING
  fprintf(stderr, "xfree received NULL\n");
#endif
}
