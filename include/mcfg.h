// mcfg.h ; marie config format parser header
// for MCFG/2
//
// Copyright (c) 2023, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef MCFG_H
#define MCFG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MCFG_2_VERSION "0.2.0 (develop)"

typedef enum mcfg_err {
  MCFG_OK,
  MCFG_TODO,
  MCFG_INVALID_PARSER_STATE,
  MCFG_SYNTAX_ERROR,
  MCFG_INVALID_KEYWORD,
  MCFG_END_IN_NOWHERE,
  MCFG_STRUCTURE_ERROR,
  MCFG_DUPLICATE_SECTOR,
  MCFG_DUPLICATE_SECTION,
  MCFG_DUPLICATE_FIELD,
  MCFG_DUPLICATE_DYNFIELD,
  MCFG_INVALID_TYPE,
  MCFG_NULLPTR,
  MCFG_INTEGER_OUT_OF_BOUNDS,
  MCFG_OS_ERROR_MASK = 0xf000
} mcfg_err_t;

typedef enum mcfg_field_type {
  TYPE_INVALID = -1,
  TYPE_STRING,
  TYPE_LIST,
  TYPE_BOOL,
  TYPE_I8,
  TYPE_U8,
  TYPE_I16,
  TYPE_U16,
  TYPE_I32,
  TYPE_U32,
} mcfg_field_type_t;

typedef enum mcfg_boolean {
  BOOL_FALSE = 0,
  BOOL_TRUE = 1,
} mcfg_boolean_t;

typedef struct mcfg_field {
  char *name;
  mcfg_field_type_t type;
  void *data;
  size_t size;
} mcfg_field_t;

typedef struct mcfg_list {
  mcfg_field_type_t type;
  size_t field_count;
  mcfg_field_t *fields;
} mcfg_list_t;

typedef struct mcfg_section {
  char *name;
  size_t field_count;
  mcfg_field_t *fields;
} mcfg_section_t;

typedef struct mcfg_sector {
  char *name;
  size_t section_count;
  mcfg_section_t *sections;
} mcfg_sector_t;

typedef struct mcfg_file {
  size_t sector_count;
  mcfg_sector_t *sectors;

  size_t dynfield_count;
  mcfg_field_t *dynfields;
} mcfg_file_t;

typedef struct mcfg_parser_ctxt {
  mcfg_file_t *target_file;
  mcfg_sector_t *target_sector;
  mcfg_section_t *target_section;
  mcfg_field_t *target_field;

  uint32_t linenum;
  char *file_path;
} mcfg_parser_ctxt_t;

typedef struct mcfg_data_parse_result {
  mcfg_err_t error;

  char *parse_end;

  bool multiline;
  void *data;
  size_t size;
} mcfg_data_parse_result_t;

typedef enum mcfg_token {
  TOKEN_INVALID = -1,
  TOKEN_SECTOR,
  TOKEN_SECTION,
  TOKEN_END,
  TOKEN_COMMENT,
  TOKEN_STR,
  TOKEN_LIST,
  TOKEN_BOOL,
  TOKEN_I8,
  TOKEN_U8,
  TOKEN_I16,
  TOKEN_U16,
  TOKEN_I32,
  TOKEN_U32,
  TOKEN_EMPTY,
} mcfg_token_t;

//-----------------------------------------------------------------------------
// Get the according string name/description for the input
//
// Params:
// err The error enum value
//
// Returns:
// Matching string name/description for input; "invalid error code" if no
// matching string could be found.
// Inputs which match MCFG_OS_ERROR_MASK will return the return value of
// strerror and require the return value to be freed after usage.
char *mcfg_err_string(mcfg_err_t err);

//------------------------------------------------------------------------------
// Get the size for the given type in bytes.
//
// Params:
// type The type to get the size of
//
// Returns:
// -1 if the type is invalid or its size is dynamic. Otherwise a positive number
// indicating the size of the datatype in bytes.
ssize_t mcfg_sizeof(mcfg_field_type_t type);

//------------------------------------------------------------------------------
// Convert the input string to its matching mcfg_field_type enum.
//
// Params:
// strtype The string for which to find the matching mcfg_field_type enum
//
// Returns:
// Matching mcfg_field_type enum. Returns TYPE_INVALID if no match could be
// found.
mcfg_field_type_t mcfg_str_to_type(char *strtype);

//------------------------------------------------------------------------------
// Gets the count of tokens in the input string
//
// Params:
// in The string for which to count the tokens
//
// Returns:
// The amount of tokens found in the string, space-seperated.
size_t mcfg_get_token_count(char *in);

//------------------------------------------------------------------------------
// Gets the token at index from string in.
//
// Params:
// in The string from which to get the token
// index The 0-based index of the token to be grabbed.
//
// Returns:
// The token at index. If the string is emtpy or the index invalid an empty
// string is returned.
// Every return value is allocated on the heap so it has to be freed.
char *mcfg_get_token_raw(char *in, uint16_t index);

//------------------------------------------------------------------------------
// Gets the mcfg_token enum value for token at index from string in.
//
// Params:
// in The string from which to get the token
// index The 0-based index of the token to be grabbed.
//
// Returns:
// The mcfg_token enum value for the token at index. Returns TOKEN_INVALID if
// index is invalid, input string is empty/NULL or no valid token could be found
// at index.
mcfg_token_t mcfg_get_token(char *in, uint16_t index);

//------------------------------------------------------------------------------
// Parses a field-declaration.
//
// Params:
// type The type of the field
// str The entire line of the field declaration.
//
// Returns:
// see declaration of struct mcfg_data_parse_result
mcfg_data_parse_result_t mcfg_parse_field_data(mcfg_field_type_t type,
                                               char *str);

//------------------------------------------------------------------------------
// Parse a line of mcfg
//
// Params:
// line The line to be parsed
// ctxt The parser context in which the line is to be parsed
//
// Returns:
// MCFG_OK if no errors occured, for other return values see the declaration of
// mcfg_err_t.
mcfg_err_t mcfg_parse_line(char *line, mcfg_parser_ctxt_t *ctxt);

//------------------------------------------------------------------------------
// Parse a file from disk
//
// Params:
// path The path to the file
// file The mcfg_file struct into which the file should be parsed
// ctxt_out Location for the parser context to be put. Can be NULL
//
// Returns:
// MCFG_OK if no errors occured, for other return values see the declaration of
// mcfg_err_t.
mcfg_err_t mcfg_parse_file_ctxto(char *path, mcfg_file_t *file,
                                 mcfg_parser_ctxt_t **ctxt_out);

//------------------------------------------------------------------------------
// Parse a file from disk
//
// Params:
// path The path to the file
// file The mcfg_file struct into which the file should be parsed
//
// Returns:
// MCFG_OK if no errors occured, for other return values see the declaration of
// mcfg_err_t.
mcfg_err_t mcfg_parse_file(char *path, mcfg_file_t *file);

//------------------------------------------------------------------------------
// Add a sector to a file
//
// Params:
// file The mcfg_file struct into which the sector should be added
// name The name of the sector to be added
//
// Returns:
// MCFG_OK if no errors occured, MCFG_DUPLICATE_SECTOR if a sector with given
// name already exists in file.
mcfg_err_t mcfg_add_sector(mcfg_file_t *file, char *name);

//------------------------------------------------------------------------------
// Add a section to a sector
//
// Params:
// sector The mcfg_sector struct into which the section should be added
// name The name of the section to be added
//
// Returns:
// MCFG_OK if no errors occured, MCFG_DUPLICATE_SECTION if a section with given
// name already exists in sector.
mcfg_err_t mcfg_add_section(mcfg_sector_t *sector, char *name);

//------------------------------------------------------------------------------
// Add a dynfield to a file
//
// Params:
// file The mcfg_file struct into which the dynfield should be added
// type The datatype of the dynfield which is to be added
// name The name of the dynfield which is to be added
// data Pointer to the data of the dynfield which is to be added
// size The size of data in bytes
//
// Returns:
// MCFG_OK if no errors occured, MCFG_DUPLICATE_DYNFIELD if a dynfield with
// the given name already exists in the file.
mcfg_err_t mcfg_add_dynfield(mcfg_file_t *file, mcfg_field_type_t type,
                             char *name, void *data, size_t size);

//------------------------------------------------------------------------------
// Add a field to a section
//
// Params:
// section The mcfg_section struct into which the field should be added
// type The datatype of the field which is to be added
// name The name of the field which is to be added
// data Pointer to the data of the field which is to be added
// size The size of data in bytes
//
// Returns:
// MCFG_OK if no errors occured, MCFG_DUPLICATE_FIELD if a field with given name
// already exists in section.
mcfg_err_t mcfg_add_field(mcfg_section_t *section, mcfg_field_type_t type,
                          char *name, void *data, size_t size);

//------------------------------------------------------------------------------
// Add a field to a list
//
// Params:
// list The mcfg_list to add the field to
// size The size of the data of the new field
// data Pointer to the data of the new field
//
// Returns:
// MCFG_OK if no errors occured, MCFG_NULLPTR if list or data is NULL.
mcfg_err_t mcfg_add_list_field(mcfg_list_t *list, size_t size, void *data);

//------------------------------------------------------------------------------
// Get the sector with name from file
//
// Params:
// file The file from which the sector is to be grabbed
// name The name of the sector
//
// Returns:
// Pointer to the sector, NULL if no sector with given name could be found.
mcfg_sector_t *mcfg_get_sector(mcfg_file_t *file, char *name);

//------------------------------------------------------------------------------
// Get the section with name from sector
//
// Params:
// sector The sector from which the section is to be grabbed
// name The name of the section
//
// Returns:
// Pointer to the section, NULL if no section with given name could be found.
mcfg_section_t *mcfg_get_section(mcfg_sector_t *sector, char *name);

//------------------------------------------------------------------------------
// Get the dynamically generated fiekd with name from file
//
// Params:
// file The file from which the field is to be grabbed
// name The name of the dynfield.
//
// Returns:
// Pointer to the field, NULL if no field with given name could be found.
mcfg_field_t *mcfg_get_dynfield(mcfg_file_t *file, char *name);

//------------------------------------------------------------------------------
// Get the field with name from section
//
// Params:
// section The section from which the field is to be grabbed
// name The name of the field
//
// Returns:
// Pointer to the field, NULL if no field with given name could be found.
mcfg_field_t *mcfg_get_field(mcfg_section_t *section, char *name);

//------------------------------------------------------------------------------
// Free the contents of given list
//
// Params:
// list The list of which the contents should be freed
void mcfg_free_list(mcfg_list_t *list);

//------------------------------------------------------------------------------
// Free the contents of given field
//
// Params:
// field The field of which the contents should be freed
void mcfg_free_field(mcfg_field_t *field);

//------------------------------------------------------------------------------
// Free the contents of given section
//
// Params:
// section The section of which the contents should be freed
void mcfg_free_section(mcfg_section_t *section);

//------------------------------------------------------------------------------
// Free the contents of given sector
//
// Params:
// sector The sector of which the contents should be freed
void mcfg_free_sector(mcfg_sector_t *sector);

//------------------------------------------------------------------------------
// Free the given file
//
// Params:
// file The file which should be freed.
void mcfg_free_file(mcfg_file_t *file);

#endif // ifndef MCFG_H
