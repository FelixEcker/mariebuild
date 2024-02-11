// mcfg_util ; marie config format utility implementation
// for MCFG/2
//
// Copyright (c) 2023, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "mcfg_util.h"

#include "mcfg.h"
#include "mcfg_shared.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t _size_t_max(size_t a, size_t b) { return a > b ? a : b; }

// Helper function to append and resize a heap allocated string
void _append_char(char **dest, size_t wix, size_t *dest_size, char chr) {
  if (dest == NULL || *dest == NULL) {
    return;
  }

  if (wix >= *dest_size) {
    size_t size_diff = wix - *dest_size;
    size_t new_size =
        *dest_size + _size_t_max(MCFG_EMBED_FORMAT_RESIZE_AMOUNT, size_diff);

    *dest = realloc_or_die(*dest, new_size);
    *dest_size = new_size;
  }

  (*dest)[wix] = chr;
}

size_t _append_str(char **dest, size_t wix, size_t *dest_size, char *src) {
  if (dest == NULL || *dest == NULL || src == NULL) {
    return 0;
  }

  for (size_t ix = 0; ix < strlen(src); ix++) {
    _append_char(dest, wix, dest_size, src[ix]);
    wix++;
  }

  return wix;
}

// Copy src until delimiter char is hit
char *_strcpy_until(char *src, char delimiter) {
  int offs = 0;
  while (offs < strlen(src)) {
    if (src[offs] == delimiter)
      break;
    offs++;
  }

  if (offs == 0)
    return strdup("\0");

  char *res = malloc(offs + 1);
  memcpy(res, src, offs);
  res[offs] = 0;

  return res;
}

// Copy backwards from src until src_org or the delimiter char is hit.
char *_bstrcpy_until(char *src, char *src_org, char delimiter) {
  int offs = 0;
  while ((src - offs) > src_org) {
    if ((src - offs)[0] == delimiter)
      break;
    offs++;
  }

  if (offs == 0)
    return strdup("\0");

  char *res = malloc(offs + 1);
  memcpy(res, src - offs + 1, offs);
  res[offs] = 0;

  return res;
}

char *_find_prev(char *src, char *src_org, char delimiter) {
  int offs = 0;
  while ((src - offs) > src_org) {
    if ((src - offs)[0] == delimiter)
      return src - offs;
    offs++;
  }

  return src - offs;
}

// Helper function for path relativity
mcfg_path_t _insert_path_elems(mcfg_path_t src, mcfg_path_t rel) {
  if (src.sector == NULL) {
    src.sector = rel.sector != NULL ? strdup(rel.sector) : strdup("(null)");
    src.absolute = true;
  }

  if (src.section == NULL)
    src.section = rel.section != NULL ? strdup(rel.section) : strdup("(null)");

  if (src.field == NULL)
    src.field = rel.field != NULL ? strdup(rel.field) : strdup("(null)");

  return src;
}

mcfg_path_t mcfg_parse_path(char *path) {
  mcfg_path_t ret = {.absolute = false,
                     .dynfield_path = false,
                     .sector = NULL,
                     .section = NULL,
                     .field = NULL};

  if (path == NULL)
    return ret;

  const char *path_seperator = "/";

  ssize_t element_count = 0;
  char **elements;
  bool absolute = path[0] == path_seperator[0];

  char *strtok_saveptr;
  char *tok = strtok_r(path, path_seperator, &strtok_saveptr);

  while (tok != NULL) {
    if (element_count == 0) {
      elements = malloc_or_die(sizeof(char *));
    } else {
      elements = realloc_or_die(elements, sizeof(char *) * (element_count + 1));
    }

    elements[element_count] = malloc_or_die(strlen(tok) + 1);
    strcpy(elements[element_count], tok);

    element_count++;
    tok = strtok_r(NULL, path_seperator, &strtok_saveptr);
  }

  if (element_count == 0)
    return ret;

  ret.absolute = absolute;

  if (absolute) {
    ret.sector = elements[0];
    if (element_count > 1)
      ret.section = elements[1];
    if (element_count > 2)
      ret.field = elements[2];

    goto exit;
  }

  if (element_count == 1) {
    if (elements[0][0] == '%' && elements[0][strlen(elements[0]) - 1] == '%') {
      ret.dynfield_path = true;

      size_t newsize = strlen(elements[0]) - 1;
      char *new = malloc_or_die(newsize);
      memcpy(new, elements[0] + 1, newsize - 1);
      new[newsize - 1] = 0;

      free(elements[0]);
      elements[0] = new;
    }

    ret.field = elements[0];
    goto exit;
  }

  ret.field = elements[element_count - 1];
  if (element_count - 2 > -1)
    ret.section = elements[element_count - 2];
  if (element_count - 3 > -1)
    ret.sector = elements[element_count - 3];

exit:
  free(elements);
  return ret;
}

char *mcfg_path_to_str(mcfg_path_t path) {
  size_t size = path.absolute ? 2 : 1;
  if (path.sector != NULL)
    size += strlen(path.sector) + 1;

  if (path.section != NULL)
    size += strlen(path.section) + 1;

  if (path.field != NULL)
    size += strlen(path.field);

  char *out = malloc_or_die(size);
  size_t offs = 0;

  if (path.absolute) {
    out[0] = '/';
    offs++;
  }

  if (path.sector != NULL) {
    strcpy(out + offs, path.sector);
    offs += strlen(path.sector);
    if (path.section != NULL) {
      out[offs] = '/';
      offs++;
    }
  }

  if (path.section != NULL) {
    strcpy(out + offs, path.section);
    offs += strlen(path.section);
    if (path.field != NULL) {
      out[offs] = '/';
      offs++;
    }
  }

  if (path.field != NULL)
    strcpy(out + offs, path.field);

  return out;
}

mcfg_field_t *mcfg_get_field_by_path(mcfg_file_t *file, mcfg_path_t path) {
  if (path.dynfield_path)
    return mcfg_get_dynfield(file, path.field);

  if (!path.absolute)
    return NULL;

  if (path.sector == NULL || path.section == NULL || path.field == NULL)
    return NULL;

  mcfg_sector_t *sector = mcfg_get_sector(file, path.sector);
  if (sector == NULL)
    return NULL;

  mcfg_section_t *section = mcfg_get_section(sector, path.section);
  if (section == NULL)
    return NULL;

  mcfg_field_t *field = mcfg_get_field(section, path.field);
  return field;
}

char *mcfg_data_to_string(mcfg_field_t field) {
  int64_t num = 0;
  switch (field.type) {
  case TYPE_LIST:
    return mcfg_list_as_string(*((mcfg_list_t *)field.data));
  case TYPE_STRING:
    return strdup(mcfg_data_as_string(field));
  case TYPE_BOOL:
    return strdup(mcfg_data_as_bool(field) ? "true" : "false");
  case TYPE_U8:
    num = mcfg_data_as_u8(field);
    break;
  case TYPE_I8:
    num = mcfg_data_as_i8(field);
    break;
  case TYPE_U16:
    num = mcfg_data_as_u16(field);
    break;
  case TYPE_I16:
    num = mcfg_data_as_i16(field);
    break;
  case TYPE_U32:
    num = mcfg_data_as_u32(field);
    break;
  case TYPE_I32:
    num = mcfg_data_as_i32(field);
    break;
  default:
    return strdup("(invalid type)");
  }

  // mystic string length calculation for number conversion
  // has a "slight" overhead (:
  char *number_ret = malloc((size_t)floor(log10((double)INT64_MAX)));
  sprintf(number_ret, "%ld", num);
  return number_ret;
}

char *mcfg_format_list(mcfg_list_t list, char *prefix, char *postfix) {
  if (list.field_count == 0 || list.fields == NULL)
    return strdup("");

  char space[2] = " ";
  size_t base_alloc_size = strlen(prefix) + strlen(postfix) + sizeof(space);

  char *seperator = malloc_or_die(base_alloc_size);
  strcpy(seperator, postfix);
  strcpy(seperator + strlen(postfix), space);
  strcpy(seperator + strlen(postfix) + strlen(space), prefix);

  size_t cpy_offs = 0;
  char *tmp = mcfg_data_to_string(list.fields[0]);
  char *out = malloc_or_die(base_alloc_size + strlen(tmp));

  strcpy(out, prefix);
  cpy_offs += strlen(prefix);
  strcpy(out + cpy_offs, tmp);
  cpy_offs += strlen(tmp);
  free(tmp);

  for (size_t ix = 1; ix < list.field_count; ix++) {
    strcpy(out + cpy_offs, seperator);
    cpy_offs += strlen(seperator);
    tmp = mcfg_data_to_string(list.fields[ix]);
    out =
        realloc_or_die(out, strlen(out) + strlen(tmp) + sizeof(seperator) + 1);
    strcpy(out + cpy_offs, tmp);
    cpy_offs = strlen(out);
    free(tmp);
  }

  size_t prev_end = strlen(out);
  out = realloc_or_die(out, strlen(out) + strlen(postfix) + 1);
  strcpy(out + prev_end, postfix);

  free(seperator);

  return out;
}

char *mcfg_list_as_string(mcfg_list_t list) {
  if (list.field_count == 0 || list.fields == NULL)
    return strdup("");

  size_t cpy_offs = 0;
  char *tmp = mcfg_data_to_string(list.fields[0]);
  char seperator[3] = ", ";
  char *out = malloc_or_die(strlen(tmp) + sizeof(seperator));

  strcpy(out, tmp);
  cpy_offs += strlen(out);
  free(tmp);

  for (size_t ix = 1; ix < list.field_count; ix++) {
    strcpy(out + cpy_offs, seperator);
    cpy_offs += strlen(seperator);
    tmp = mcfg_data_to_string(list.fields[ix]);
    out =
        realloc_or_die(out, strlen(out) + strlen(tmp) + sizeof(seperator) + 1);
    strcpy(out + cpy_offs, tmp);
    cpy_offs = strlen(out);
    free(tmp);
  }

  return out;
}

mcfg_list_t *mcfg_data_as_list(mcfg_field_t field) {
  if (field.data != NULL && field.type == TYPE_LIST)
    return (mcfg_list_t *)field.data;
  return NULL;
}

char *mcfg_data_as_string(mcfg_field_t field) {
  if (field.data != NULL && field.type == TYPE_STRING)
    return (char *)field.data;
  return NULL;
}

char *mcfg_format_field_embeds(mcfg_field_t field, mcfg_file_t file,
                               mcfg_path_t relativity) {
  if (field.data == NULL || field.type != TYPE_STRING)
    return NULL;

  char *input = mcfg_data_as_string(field);
  return mcfg_format_field_embeds_str(input, file, relativity);
}

char *mcfg_format_field_embeds_str(char *input, mcfg_file_t file,
                                   mcfg_path_t relativity) {
  size_t input_len = strlen(input);

  char *result = malloc_or_die(input_len + 1);
  size_t current_result_size = input_len + 1;

  size_t wix = 0;
  bool escaping = false;

  bool building_embed_opening = false;
  bool building_field_name = false;
  char *embedded_field;
  size_t embedded_field_name_start = 0;

  for (size_t ix = 0; ix < input_len; ix++) {
    switch (input[ix]) {
    case '\\':
      if (escaping) {
        _append_char(&result, wix, &current_result_size, input[ix]);
        wix++;
      }
      escaping = !escaping;
      break;
    case MCFG_EMBED_PREFIX:
      if (!escaping) {
        building_embed_opening = true;
      } else {
        escaping = false;

        _append_char(&result, wix, &current_result_size, input[ix]);
        wix++;
      }
      break;
    case MCFG_EMBED_OPENING:
      if (!escaping && building_embed_opening) {
        building_field_name = true;
        embedded_field_name_start = ix + 1;
      } else {
        _append_char(&result, wix, &current_result_size, input[ix]);
        wix++;
      }

      building_embed_opening = false;
      break;
    case MCFG_EMBED_CLOSING:
      if (building_field_name) {
        building_field_name = false;

        size_t _len = ix - embedded_field_name_start;
        embedded_field = malloc_or_die(_len + 1);
        memcpy(embedded_field, input + embedded_field_name_start, _len);
        embedded_field[_len] = 0;

        mcfg_path_t path =
            _insert_path_elems(mcfg_parse_path(embedded_field), relativity);
        mcfg_field_t *_field = mcfg_get_field_by_path(&file, path);

        char *formatted_contents;
        if (_field == NULL) {
          formatted_contents = strdup("(nullptr)");
          goto case_embed_closing_end;
        }

        if (_field->type == TYPE_LIST) {
          char *prefix = remove_newline(_bstrcpy_until(
              input + embedded_field_name_start - 3, input, ' '));
          char *postfix = remove_newline(_strcpy_until(input + ix + 1, ' '));
          formatted_contents =
              mcfg_format_list(*mcfg_data_as_list(*_field), prefix, postfix);

          wix = _find_prev(input + ix, input, ' ') - input + 1;
          ix = (size_t)(strchr(input + ix, ' ') - input) - 1;

          free(prefix);
          free(postfix);
        } else {
          formatted_contents = mcfg_data_to_string(*_field);
        }

        char *old_formatted_contents = formatted_contents;

        formatted_contents =
            mcfg_format_field_embeds_str(formatted_contents, file, relativity);

        free(old_formatted_contents);

      case_embed_closing_end:
        wix =
            _append_str(&result, wix, &current_result_size, formatted_contents);

        free(formatted_contents);
        free(embedded_field);
        free(path.sector);
        free(path.section);
        free(path.field);
        break;
      }

      _append_char(&result, wix, &current_result_size, input[ix]);
      wix++;
      break;
    default:
      if (!building_field_name) {
        _append_char(&result, wix, &current_result_size, input[ix]);
        wix++;
      }
      break;
    }
  }
  _append_char(&result, wix, &current_result_size, 0);

  return result;
}

int mcfg_data_as_int(mcfg_field_t field) {
  if (mcfg_sizeof(field.type) <= 0 || field.size < 1)
    return 0;

  return (int)*(int *)field.data;
}

mcfg_boolean_t mcfg_data_as_bool(mcfg_field_t field) {
  if (field.type != TYPE_BOOL)
    return false;

  return (mcfg_boolean_t) * (mcfg_boolean_t *)field.data;
}

uint8_t mcfg_data_as_u8(mcfg_field_t field) {
  if (field.type != TYPE_U8)
    return 0;

  return (uint8_t) * (uint8_t *)field.data;
}

int8_t mcfg_data_as_i8(mcfg_field_t field) {
  if (field.type != TYPE_I8)
    return 0;

  return (int8_t) * (int8_t *)field.data;
}

uint16_t mcfg_data_as_u16(mcfg_field_t field) {
  if (field.type != TYPE_U16)
    return 0;

  return (uint16_t) * (uint16_t *)field.data;
}

int16_t mcfg_data_as_i16(mcfg_field_t field) {
  if (field.type != TYPE_I16)
    return 0;

  return (int16_t) * (int16_t *)field.data;
}

uint32_t mcfg_data_as_u32(mcfg_field_t field) {
  if (field.type != TYPE_U32)
    return 0;

  return (uint32_t) * (uint32_t *)field.data;
}

int32_t mcfg_data_as_i32(mcfg_field_t field) {
  if (field.type != TYPE_I32)
    return 0;

  return (int32_t) * (int32_t *)field.data;
}
