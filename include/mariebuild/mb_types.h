/*
 * mariebuild: mb_types.h ; author: Marie Eckert
 *
 * Commonly used datatypes
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_TYPES_H
#define MB_TYPES_H

typedef struct mb_field {
  char *name;
  char *value;
} mb_field;

typedef struct mb_section {
  char     *name;
  int      section_type;
  char     *lines;
  int      field_count;
  mb_field *fields;
} mb_section;

typedef struct mb_sector {
  char       *name;
  int        section_count;
  mb_section *sections;
} mb_sector;

typedef struct mb_file {
  char      *path;
  int       line;
  int       sector_count;
  mb_sector *sectors;
} mb_file;

#endif
