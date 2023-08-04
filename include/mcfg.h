/*
 * mcfg.h ; author: Marie Eckert
 *
 * mcfg parser and utilities.
 * This header and its source file originally stem from mariebuild and have
 * been partially rewritten to be used as a general configuration file.
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mcfg/blob/master/LICENSE>
 */

#ifndef MCFG_H
#define MCFG_H

#define MCFG_OK                      0
#define MCFG_ERR_UNKNOWN             0x00000001
#define MCFG_PERR_MASK               0x10000000
#define MCFG_PERR_MISSING_REQUIRED   0x10000001
#define MCFG_PERR_DUPLICATE_SECTION  0x10000002
#define MCFG_PERR_DUPLICATE_SECTOR   0x10000003
#define MCFG_PERR_DUPLICATE_FIELD    0x10000004
#define MCFG_PERR_INVALID_IDENTIFIER 0x10000005
#define MCFG_PERR_INVALID_SYNTAX     0x10000006
#define MCFG_PERR_INVALID_FTYPE      0x10000007
#define MCFG_PERR_INVALID_STYPE      0x10000008
#define MCFG_ERR_MASK_ERRNO          0xf0000000

/* Used to set the type of a field. If the type ever is FT_UNKOWN an error
 * should be thrown
 */
typedef enum mcfg_ftype {
  FT_STRING,
  FT_LIST,
  FT_UNKNOWN
} mcfg_ftype;

/* Used to set the type of a sector. If the type ever is ST_UNKNOWN an error
 * should be thrown
 */
typedef enum mcfg_stype {
  ST_FIELDS,
  ST_LINES,
	ST_UNKNOWN
} mcfg_stype;

/* Holds a field specified within a config section.
 */
typedef struct mcfg_field {
  mcfg_ftype  type;
  char       *name;
  char       *value;
} mcfg_field;

/* Defines a section of a sector within a mcfg file
 */
typedef struct mcfg_section {
  mcfg_stype  type;
  char       *name;
  int         section_type;
  char       *lines;
  int         field_count;
  mcfg_field *fields;
} mcfg_section;

/* Defines a sector of a mcfg file
 */
typedef struct mcfg_sector {
  char       *name;
  int        section_count;
  mcfg_section *sections;
} mcfg_sector;

/* C-Representation of a mcfg file
 */
typedef struct mcfg_file {
  char      *path;
  int       line;
  int       sector_count;
  mcfg_sector *sectors;
} mcfg_file;

/* Completely and recursively free a mcfg_file struct
 */
void free_mcfg_file(mcfg_file* file);

/* Parsing Functions */
/* NOTE: After using any of these registering functions, pointers to members
 *       of the targeted mcfg-file need to be reassigned since registering
 *       breaks the old pointers.
 */
int register_sector(struct mcfg_file* file, char *name);
int register_section(struct mcfg_sector* sector, mcfg_stype type, char *name);
int register_field(struct mcfg_section* section, mcfg_ftype type,
                       char *name, char *value);

int parse_line(struct mcfg_file* file, char *line);

/* Parses the file under the path in file->path line by line,
 * will return MCFG_OK if there were no errors.
 */
int parse_file(struct mcfg_file* file);

/* Navigation Functions */

mcfg_sector *find_sector(struct mcfg_file* file, char *sector_name);
mcfg_section *find_section(struct mcfg_file* file, char *path);
mcfg_field *find_field(struct mcfg_file* file, char *path);

/* Formats the contents of a list field.

 * Parameters:
 *   file   : The file structure from which to take the files field
 *   field  : The field to be formatted
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
char *format_list_field(struct mcfg_file file, mcfg_field field, char *context,
                         char *in, int in_offs, int len);

/*
 * Resolve the field values in the given input string by replacing field-
 * references with their corresponding values.
 *
 * Parameters:
 *   file       : The build file structure containing the sectors,
 *              | sections, and fields.
 *   in         : The input string to be resolved.
 *   context    : The context in which the field references should be resolved.
 *              | It specifies the path prefix for the field lookups.
 *   leave_lists: If set to 1 lists are not formatted, any other value will
 *                cause lists to be formatted
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
 *     found in the mcfg file.
 *   - Field references can include the context prefix to specify the
 *     location of the field lookup.
 *   - If a field reference cannot be resolved or a field value is NULL,
 *     it will not be replaced in the output string.
 *   - The resolved string is returned as a dynamically allocated string.
 *     The caller must free the memory allocated for the resolved string
 *     when it's no longer needed.
 */
char *resolve_fields(struct mcfg_file file, char *in, char *context, 
                       int leave_lists);

#endif
