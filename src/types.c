// types.c ; mariebuild util functions for shared type-definitions
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "types.h"

#include <stddef.h>
#include <string.h>

struct build_type_id {
  char *name;
  build_type_t value;
};

struct build_type_id build_type_lookup[] = {
    {.name = "incremental", .value = BUILD_TYPE_INCREMENTAL},
    {.name = "full", .value = BUILD_TYPE_FULL}};

const size_t BUILD_TYPE_LOOKUP_SIZE = sizeof(build_type_lookup);

build_type_t str_to_build_type(char *src, build_type_t fallback) {
  for (size_t ix = 0; ix < BUILD_TYPE_LOOKUP_SIZE; ix++) {
    if (strcmp(src, build_type_lookup[ix].name) == 0)
      return build_type_lookup[ix].value;
  }

  return fallback;
}
