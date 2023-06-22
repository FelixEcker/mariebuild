/* 
 * mariebuild: mb_parse.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_PARSE_H
#define MB_PARSE_H

#define MB_OK          0x00000000
#define MB_ERR_UNKNOWN 0x00000001

/* Parsing Errors */
#define MB_PERR_MISSING_REQUIRED   0x00000101
#define MB_PERR_DUPLICATE_SECTION  0x00000102
#define MB_PERR_DUPLICATE_SECTOR   0x00000103
#define MB_PERR_DUPLICATE_FIELD    0x00000104
#define MB_PERR_INVALID_IDENTIFIER 0x00000105
#define MB_PERR_INVALID_SYNTAX     0x00000106

/* Mask if a system error occured which reports its error in errno */
#define MB_SERR_MASK_ERRNO       0x10000000

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
  int       sector_count;
  mb_sector *sectors;
} mb_file;

void free_build_file(mb_file* file);

int register_sector(struct mb_file* file, char *name);
int register_section(struct mb_sector* sector, char *name);
int register_field(struct mb_section* section, char *name, char *value);

int parse_line(struct mb_file* file, char *line);
int parse_file(struct mb_file* file);

#endif
