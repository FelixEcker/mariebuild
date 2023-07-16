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

/* Parses the file under the path in build_file->path line by line,
 * will return MB_OK if there were no errors.
 *
 * Refer to mb_utils.h for error codes.
 */
int parse_file(struct mb_file* file);

/* Navigation Functions */

mb_sector *find_sector(struct mb_file* file, char *sector_name);
mb_section *find_section(struct mb_file* file, char *path);
mb_field *find_field(struct mb_file* file, char *path);

/* Formats the contents of the .config/mariebuild/files field.

 * Parameters:
 *   file   : The file structure from which to take the files field
 *   context: Path of the section which is to be used for local fields
 *   in     : The string in which the files field is embedded
 *   in_offs: Offset for the input string
 *   len    : Length of the embed including $()
 *
 * Returns:
 *   A dynamically allocated string with the formatted result.
 *   The caller is responsible for freeing the memory.
 *
 * Notes:
 *   - The list is inserted with space-seperation. Chars which come immediatly
 *     after or before the embed are post- or prefixed to every file.
 *
 * Example:
 *  files = 'file1:file2'
 *  format_files_field(file, ".config/mariebuild", "out/$(files).o", offs, len)
 *  = out/file1.o out/file2.o
 */
char *format_files_field(struct mb_file file, char *context,
                         char *in, int in_offs, int len);
/*
 * Resolve the field values in the given input string by replacing field-
 * references with their corresponding values.
 *
 * Parameters:
 *   file   : The build file structure containing the sectors,
 *          | sections, and fields.
 *   in     : The input string to be resolved.
 *   context: The context in which the field references should be resolved.
 *          | It specifies the path prefix for the field lookups.
 *
 * Returns:
 *   A dynamically allocated string containing the resolved input string.
 *   The caller is responsible for freeing the memory allocated for the
 *   resolved string.
 *
 * Notes:
 *   - The input string may contain field references in the format
 *     "$(path/to/field)".
 *   - Field references will be replaced with their corresponding values
 *     found in the build file.
 *   - Field references can include the context prefix to specify the
 *     location of the field lookup.
 *   - If a field reference cannot be resolved or a field value is NULL,
 *     it will not be replaced in the output string.
 *   - The resolved string is returned as a dynamically allocated string.
 *     The caller must free the memory allocated for the resolved string
 *     when it's no longer needed.
 */
char *resolve_fields(struct mb_file file, char *in, char *context);

#endif
