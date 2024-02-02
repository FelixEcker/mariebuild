// xmem.h ; mariebuild memory helpers header
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef XMEM_H
#define XMEM_H

#if !defined(XMEM_LOGGING)
#define XMEM_LOGGING 1
#endif

#include <stddef.h>

void *xmalloc(size_t size);

void *xrealloc(void *org, size_t size);

void xfree(void *ptr);

#endif // #ifndef XMEM_H
