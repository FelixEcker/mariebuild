/*
 * mariebuild: mb_parse.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_PARSE_H
#define MB_PARSE_H

#include <mariebuild/mb_types.h>

void free_build_file(mb_file* file);

/* Parsing Functions */
/* NOTE: After using any of these registering functions, pointers to members 
 *       of the targeted build-file need to be reassigned since registering 
 *       breaks the old pointers.
 */
int register_sector(struct mb_file* file, char *name);
int register_section(struct mb_sector* sector, char *name);
int register_field(struct mb_section* section, char *name, char *value);

int parse_line(struct mb_file* file, char *line);
int parse_file(struct mb_file* file);

/* Navigation Functions */

mb_sector *find_sector(struct mb_file* file, char *sector_name);
mb_section *find_section(struct mb_file* file, char *path);
mb_field *find_field(struct mb_file* file, char *path);

/* Resolve all field embeds within the string "in"
 * context : Path of the section in which the field embeds are being resolved.
 *           This is used for resolving relative field names.
 */
char *resolve_fields(struct mb_file file, char *in, char *context);

#endif
