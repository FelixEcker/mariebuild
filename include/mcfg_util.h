/* mcfg_util.h ; marie config format utility header
 * for MCFG/2
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#ifndef MCFG_UTIL_H
#define MCFG_UTIL_H

#include "mcfg.h"

#define MCFG_EMBED_FORMAT_RESIZE_AMOUNT 16

#define MCFG_EMBED_PREFIX '$'
#define MCFG_EMBED_OPENING '('
#define MCFG_EMBED_CLOSING ')'

typedef struct mcfg_path {
  bool absolute;
  bool dynfield_path;

  char *sector;
  char *section;
  char *field;
} mcfg_path_t;

/**
 * @brief Convert a string path into a mcfg_path struct
 * @param path The path which should be converted
 * @return The path parsed into a mcfg_path struct, each element is a seperatly
 * heap-allocated string. If this function fails, all elements will be null in
 * the returned value.
 */
mcfg_path_t mcfg_parse_path(char *path);

/**
 * @brief Convert a mcfg_path_t structure into its string representation
 * @param path The path to be converted
 * @return A Heap allocated string with the path structures string
 * representation
 */
char *mcfg_path_to_str(mcfg_path_t path);

/**
 * @brief Gets a field by its path
 * @param file The file in which the field lies
 * @param path The path to the field which should be grabbed. This path has to
 * be absolute!
 * @return Pointer to the field pointed to by path. If the field was not found
 * NULL will be returned.
 */
mcfg_field_t *mcfg_get_field_by_path(mcfg_file_t *file, mcfg_path_t path);

/**
 * @brief Converts the data of the given field to a string representation
 * @param field The field of which the data should be converted
 * @return Heap-Allocated string representation of the fields data.
 */
char *mcfg_data_to_string(mcfg_field_t field);

/**
 * @brief Formats a list field using the provided pre- and postfix
 * @param list The list struct of which the contents should be formatted
 * @return Heap-Allocated string of the formatted list.
 */
char *mcfg_format_list(mcfg_list_t list, char *prefix, char *postfix);

/**
 * @brief Converts a list to its string representation
 * @param list The list of which the data should be converted
 * @return Heap-Allocated string representation of the list's data.
 */
char *mcfg_list_as_string(mcfg_list_t list);

/**
 * @brief Get the data of the field as a string.
 * @param field The field of which the data should be grabbed
 * A char-pointer to the data variable in the given field. If the field's type
 * is not TYPE_STRING a NULL-Pointer will be returned.
 */
char *mcfg_data_as_string(mcfg_field_t field);

/**
 * @brief Get the data of the field as a list
 * @param field The field of which the data should be grabbed
 * A pointer to a mcfg_list_t structure formed from the data pointer in the
 * given field. If the data pointer is NULL, a NULL-Pointer will be returned.
 */
mcfg_list_t *mcfg_data_as_list(mcfg_field_t field);

/**
 * @brief Get the data of the field as an int.
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an integer. If the type is not of
 * a number variant (boolean, u8, i8, ...) or the data is a NULL-Pointer,
 * 0 will be returned as default.
 */
int mcfg_data_as_int(mcfg_field_t field);

/**
 * @brief Get the data of the field as a boolean
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as a boolean (mcfg_boolean_t). If
 * the type is not boolean or the data is a NULL-Pointer,
 * false will be returned as default.
 */
mcfg_boolean_t mcfg_data_as_bool(mcfg_field_t field);

/**
 * @brief Get the data of the field as an u8
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an uint8_t. If the field's type
 * is not TYPE_U8 or the data is a NULL-Pointer, 0 will be returned as default.
 */
uint8_t mcfg_data_as_u8(mcfg_field_t field);

/**
 * @brief Get the data of the field as an i8
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an int8_t. If the field's type
 * is not TYPE_I8 or the data is a NULL-Pointer, 0 will be returned as default.
 */
int8_t mcfg_data_as_i8(mcfg_field_t field);

/**
 * @brief Get the data of the field as an u16
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an uint16_t. If the field's type
 * is not TYPE_U16 or the data is a NULL-Pointer, 0 will be returned as default.
 */
uint16_t mcfg_data_as_u16(mcfg_field_t field);

/**
 * @brief Get the data of the field as an i16
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an int16_t. If the field's type
 * is not TYPE_I16 or the data is a NULL-Pointer, 0 will be returned as default.
 */
int16_t mcfg_data_as_i16(mcfg_field_t field);

/**
 * @brief Get the data of the field as an u32
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an uint32_t. If the field's type
 * is not TYPE_U32 or the data is a NULL-Pointer, 0 will be returned as default.
 */
uint32_t mcfg_data_as_u32(mcfg_field_t field);

/**
 * @brief Get the data of the field as an i32
 * @param field The field of which the data should be grabbed
 * The value of the fields data represented as an int32_t. If the field's type
 * is not TYPE_I32 or the data is a NULL-Pointer, 0 will be returned as default.
 */
int32_t mcfg_data_as_i32(mcfg_field_t field);

#endif // ifndef MCFG_UTIL_H
