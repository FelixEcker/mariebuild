/*
 * mariebuild: mb.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */


#include <stdlib.h>
#include <stdio.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>
#include <mariebuild/mb_utils.h>

void print_structure(struct mb_file* build_file) {
  printf("\n==========================\n\n");
  for (int i = 0; i < build_file->sector_count; i++) {
    mb_sector sector = build_file->sectors[i];
    printf("sector: %s\n", sector.name);
    printf("sections:\n");
    for (int j = 0; j < sector.section_count; j++) {
      mb_section section = sector.sections[j];
      printf("  section: %s\n", section.name);
      printf("  lines: %s\n", section.lines);
      printf("  fields:\n");
      for (int k = 0; k < section.field_count; k++) {
        mb_field field = section.fields[k];
        printf("    name: %s\n", field.name);
        printf("    value: %s\n", field.value);
      }
    }
  }
  printf("\n==========================\n");
}

int main() {
  struct mb_file* build_file = malloc(sizeof(mb_file));
  build_file->path = "./build.mb";
  int result = parse_file(build_file);

  if ((result & MB_SERR_MASK_ERRNO) == MB_SERR_MASK_ERRNO)
    result = result & 0;

  if (result != 0) {
    // The error message and code have to be split since mb_log only accepts
    // strings for formatting at the moment.
    mb_log(MB_LOGLVL_IMP, "Parsing failed: Line ");
    printf("%d\n", build_file->line);
    mb_logf(MB_LOGLVL_IMP, "Parsing failed: %s ", errcode_msg(result));
    printf("(0x%.8x)\n", result);
    mb_log(MB_LOGLVL_IMP, "Aborting build...\n");

    free_build_file(build_file);
    return result;
  }
  
  print_structure(build_file);

  struct mb_exec_params exec_params;
  exec_params.exec_script = NULL;
  exec_params.platform    = NULL;
  exec_params.force       = 0;
  exec_params.status      = 0;

  result = mb_exec_build((*build_file), exec_params);
  if (result != 0) {
    mb_logf(MB_LOGLVL_IMP, "Build failed: %s ", errcode_msg(result));
    printf("(0x%.8x)\n", result);
  } else {
    mb_log(MB_LOGLVL_STD, "Build succeeded!\n");
  }

  free_build_file(build_file);
  return result;
}
