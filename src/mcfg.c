// mcfg.c ; marie config format parser implementation
// implementation for MCFG/2
//
// Copyright (c) 2023, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------
// TODO:
// - Parse lists with element type string
// - Edge-Case Handling (checking for null-pointer, ...)
//------------------------------------------------------------------------------

#include "mcfg.h"

#include "mcfg_shared.h"

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool _integer_bounds_check(int64_t _int, mcfg_field_type_t type) {
  if (mcfg_sizeof(type) <= 0)
    return false;

  switch (type) {
  case TYPE_BOOL:
    return _int >= BOOL_FALSE && _int <= BOOL_TRUE;
  case TYPE_I8:
    return _int >= INT8_MIN && _int <= INT8_MAX;
  case TYPE_U8:
    return _int >= 0 && _int <= UINT8_MAX;
  case TYPE_I16:
    return _int >= INT16_MIN && _int <= INT16_MAX;
  case TYPE_U16:
    return _int >= 0 && _int <= UINT16_MAX;
  case TYPE_I32:
    return _int >= INT32_MIN && _int <= INT32_MAX;
  case TYPE_U32:
    return _int >= 0 && _int <= UINT32_MAX;
  default:
    return false;
  }
}

mcfg_boolean_t _strtobool(char *in) {
  if (is_string_empty(in))
    return BOOL_FALSE;

  return strcmp(in, "true") == 0 ? BOOL_TRUE : BOOL_FALSE;
}

char *mcfg_err_string(mcfg_err_t err) {
  if ((err & MCFG_OS_ERROR_MASK) == MCFG_OS_ERROR_MASK)
    return strerror(~MCFG_OS_ERROR_MASK & err);

  switch (err) {
  case MCFG_TODO:
    return "TODO";
  case MCFG_OK:
    return "Everything is OK :)";
  case MCFG_INVALID_PARSER_STATE:
    return "Invalid Parser State.";
  case MCFG_SYNTAX_ERROR:
    return "Syntax Error";
  case MCFG_INVALID_KEYWORD:
    return "Invalid Keyword/Token";
  case MCFG_END_IN_NOWHERE:
    return "Usage of \"end\" keyword in nowhere";
  case MCFG_STRUCTURE_ERROR:
    return "Invalid structure of input";
  case MCFG_DUPLICATE_SECTOR:
    return "Duplicate Sector";
  case MCFG_DUPLICATE_SECTION:
    return "Duplicate Section";
  case MCFG_DUPLICATE_FIELD:
    return "Duplicate Field";
  case MCFG_DUPLICATE_DYNFIELD:
    return "Duplicate Dynamic Field";
  case MCFG_INVALID_TYPE:
    return "Invalid Datatype";
  case MCFG_NULLPTR:
    return "NULL-Pointer";
  case MCFG_INTEGER_OUT_OF_BOUNDS:
    return "Integer value is out of bounds";
  default:
    return "invalid error code";
  }
}

ssize_t mcfg_sizeof(mcfg_field_type_t type) {
  switch (type) {
  case TYPE_BOOL:
  case TYPE_I8:
  case TYPE_U8:
    return 1;
  case TYPE_I16:
  case TYPE_U16:
    return 2;
  case TYPE_I32:
  case TYPE_U32:
    return 4;
  default:
    return -1;
  }
}

struct _mcfg_type_id {
  char *name;
  mcfg_field_type_t value;
};

const struct _mcfg_type_id TYPE_IDS[] = {
    {.name = "str", .value = TYPE_STRING}, {.name = "list", .value = TYPE_LIST},
    {.name = "bool", .value = TYPE_BOOL},  {.name = "i8", .value = TYPE_I8},
    {.name = "u8", .value = TYPE_U8},      {.name = "i16", .value = TYPE_I16},
    {.name = "u16", .value = TYPE_U16},    {.name = "i32", .value = TYPE_I32},
    {.name = "u32", .value = TYPE_U32},
};
const size_t EXISTING_TYPE_COUNT =
    sizeof(TYPE_IDS) / sizeof(struct _mcfg_type_id);

mcfg_field_type_t mcfg_str_to_type(char *strtype) {
  mcfg_field_type_t ret = TYPE_INVALID;

  strtype = strdup(strtype);
  remove_newline(strtype);

  if (is_string_empty(strtype)) {
    goto mcfg_get_token_exit;
  }

  for (size_t ix = 0; ix < EXISTING_TYPE_COUNT; ix++) {
    if (strcmp(strtype, TYPE_IDS[ix].name) == 0) {
      ret = TYPE_IDS[ix].value;
      break;
    }
  }

mcfg_get_token_exit:
  free(strtype);
  return ret;
}

struct _mcfg_token_id {
  char *name;
  mcfg_token_t value;
};

const struct _mcfg_token_id TOKEN_IDS[] = {
    {.name = "sector", .value = TOKEN_SECTOR},
    {.name = "section", .value = TOKEN_SECTION},
    {.name = "end", .value = TOKEN_END},
    {.name = ";", .value = TOKEN_COMMENT},
    {.name = "comment", .value = TOKEN_COMMENT},
    {.name = "str", .value = TOKEN_STR},
    {.name = "list", .value = TOKEN_LIST},
    {.name = "bool", .value = TOKEN_BOOL},
    {.name = "i8", .value = TOKEN_I8},
    {.name = "u8", .value = TOKEN_U8},
    {.name = "i16", .value = TOKEN_I16},
    {.name = "u16", .value = TOKEN_U16},
    {.name = "i32", .value = TOKEN_I32},
    {.name = "u32", .value = TOKEN_U32},
};
const size_t EXISTING_TOKEN_COUNT =
    sizeof(TOKEN_IDS) / sizeof(struct _mcfg_token_id);

size_t _token_position(char *in, uint16_t index) {
  size_t pos = 0;

  char *string_tok_ptr = NULL;
  char *indup = strdup(in);
  char *tok = strtok_r(indup, " ", &string_tok_ptr);

  uint16_t current_index = 0;
  while (tok != NULL) {
    if (current_index == index)
      break;

    tok = strtok_r(NULL, " ", &string_tok_ptr);
    current_index++;
  }

  if (tok != NULL)
    pos = tok - indup;

  free(indup);
  return pos;
}

size_t mcfg_get_token_count(char *in) {
  if (is_string_empty(in))
    return 0;

  size_t count = 0;
  char *string_tok_ptr = NULL;
  char *indup = strdup(in);
  char *tok = strtok_r(indup, " ", &string_tok_ptr);

  while (tok != NULL) {
    count++;
    tok = strtok_r(NULL, " ", &string_tok_ptr);
  }

  free(indup);
  return count;
}

char *mcfg_get_token_raw(char *in, uint16_t index) {
  if (is_string_empty(in))
    return strdup("");

  char *string_tok_ptr = NULL;
  char *indup = strdup(in);
  char *ret = strtok_r(indup, " ", &string_tok_ptr);

  uint16_t current_index = 0;
  while (ret != NULL) {
    if (current_index == index)
      break;

    ret = strtok_r(NULL, " ", &string_tok_ptr);
    current_index++;
  }

  if (ret == NULL) {
    free(ret);
    return strdup("");
  }

  char *aret = strdup(ret);
  free(indup);
  return aret;
}

mcfg_token_t mcfg_get_token(char *in, uint16_t index) {
  mcfg_token_t tok = TOKEN_INVALID;
  in = mcfg_get_token_raw(in, index);

  remove_newline(in);

  if (is_string_empty(in)) {
    tok = TOKEN_EMPTY;
    goto mcfg_get_token_exit;
  }

  for (size_t ix = 0; ix < EXISTING_TOKEN_COUNT; ix++) {
    if (strcmp(in, TOKEN_IDS[ix].name) == 0) {
      tok = TOKEN_IDS[ix].value;
      break;
    }
  }

mcfg_get_token_exit:
  free(in);
  return tok;
}

mcfg_data_parse_result_t _parse_string_field(char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK,
      .multiline = false,
      .data = NULL,
      .size = 0,
  };

  ret.data = malloc_or_die(strlen(str) + 1);
  if (strlen(str) == 0) {
    ret.data = malloc_or_die(1);
    ((char *)ret.data)[0] = 0;
    ret.size = 1;
    return ret;
  }

  size_t ix = 0;
  size_t wix = 0;
  for (; ix < strlen(str); ix++) {
    if (str[ix] == '\'') {
      // Disgusting!
      bool prev_char_not_quote = ix == 0 || str[ix - 1] != '\'';
      bool next_char_not_quote =
          ix + 1 < strlen(str) ? str[ix + 1] != '\'' : true;
      if (prev_char_not_quote && next_char_not_quote)
        break;
      else if (!prev_char_not_quote)
        continue;
    }

    ((char *)ret.data)[wix] = str[ix];

    wix++;
    ret.size++;

    if (str[ix] == '\n') {
      ret.multiline = true;
      break;
    }
  }
  ((char *)ret.data)[wix] = 0;
  ret.size++;
  ret.parse_end = str + ix;

  return ret;
}

mcfg_data_parse_result_t _parse_number_type_field(mcfg_field_type_t type,
                                                  char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK, .multiline = false, .data = NULL, .size = 0};

  ret.size = mcfg_sizeof(type);
  if (ret.size <= 0) {
    ret.error = MCFG_INVALID_TYPE;
    goto _parse_number_type_field_ret;
  }

  ret.data = malloc_or_die(ret.size);

  int64_t converted = 0;

  remove_newline(str);
  if (type == TYPE_BOOL)
    converted = _strtobool(str);
  else
    converted = strtol(str, NULL, 10);

  if (!_integer_bounds_check(converted, type)) {
    ret.error = MCFG_INTEGER_OUT_OF_BOUNDS;
    goto _parse_number_type_field_ret;
  }

  memcpy(ret.data, &converted, ret.size);

_parse_number_type_field_ret:
  return ret;
}

mcfg_data_parse_result_t _parse_str_list_data(mcfg_list_t *list, char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK, .multiline = false, .data = NULL, .size = 0};

  mcfg_field_type_t list_type = list->type;

  if (list_type != TYPE_STRING) {
    ret.error = MCFG_INVALID_PARSER_STATE;
    return ret;
  }

  char *parse_start = strchr(str, '\'');

  while (parse_start != NULL && parse_start[0] != 0) {
    mcfg_data_parse_result_t data_result = _parse_string_field(parse_start + 1);

    if (data_result.error != MCFG_OK) {
      ret.error = data_result.error;
      break;
    }

    ret.error = mcfg_add_list_field(list, data_result.size, data_result.data);
    if (ret.error != MCFG_OK)
      break;

    parse_start = strchr(data_result.parse_end + 1, '\'');
  }

  ret.data = list;
  ret.size = sizeof(*list);

  return ret;
}

mcfg_data_parse_result_t _parse_list_data(mcfg_list_t *list, char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK, .multiline = false, .data = NULL, .size = 0};

  mcfg_field_type_t list_type = list->type;

  if (list_type == TYPE_INVALID || list_type == TYPE_LIST) {
    ret.error = MCFG_INVALID_TYPE;
    return ret;
  }

  if (list_type == TYPE_STRING)
    return _parse_str_list_data(list, str);

  bool list_end = false;
  bool line_end = false;

  size_t tok_count = mcfg_get_token_count(str);
  size_t tok_ix = 0;
  for (; tok_ix < tok_count; tok_ix++) {
    char *str_value = mcfg_get_token_raw(str, tok_ix);

    line_end = has_newline(str_value);

    // avoid interpreting trailing spaces at line end to be list elements
    if (line_end && is_string_empty(str_value)) {
      free(str_value);
      break;
    }

    remove_newline(str_value);
    list_end = str_value[strlen(str_value) - 1] != ',';

    if (!list_end)
      str_value[strlen(str_value) - 1] = 0;

    mcfg_data_parse_result_t data_result =
        _parse_number_type_field(list_type, str_value);
    free(str_value);

    // TODO: This might cause some problems down the line, check back later
    if (data_result.error != MCFG_OK)
      return ret;

    ret.error = mcfg_add_list_field(list, data_result.size, data_result.data);
    if (ret.error != MCFG_OK)
      break;

    if (list_end)
      break;
  }

  if (ret.error != MCFG_OK)
    return ret;

  if (list_end && tok_ix < (tok_count - 1))
    ret.error = MCFG_SYNTAX_ERROR;

  if (!list_end && line_end)
    ret.multiline = true;

  ret.data = list;
  ret.size = sizeof(*list);

  return ret;
}

mcfg_data_parse_result_t _parse_list_field(char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK, .multiline = false, .data = NULL, .size = 0};

  // 0    1           2      3        4
  // list [list_type] [name] [value], [value] ...
  // minimum token count = 4

  size_t tok_count = mcfg_get_token_count(str);
  if (tok_count < 4) {
    ret.error = MCFG_SYNTAX_ERROR;
    return ret;
  }

  char *strtype = mcfg_get_token_raw(str, 1);
  mcfg_field_type_t list_type = mcfg_str_to_type(strtype);
  free(strtype);

  mcfg_list_t *list = malloc(sizeof(mcfg_list_t));
  list->type = list_type;
  list->field_count = 0;

  size_t first_val_pos = _token_position(str, 3);
  ret = _parse_list_data(list, str + first_val_pos);
  if (ret.error != MCFG_OK)
    free(list);

  return ret;
}

mcfg_data_parse_result_t mcfg_parse_field_data(mcfg_field_type_t type,
                                               char *str) {
  mcfg_data_parse_result_t ret = {
      .error = MCFG_OK,
      .multiline = false,
      .data = NULL,
      .size = 0,
  };

  if (type == TYPE_INVALID) {
    ret.error = MCFG_INVALID_TYPE;
    return ret;
  }

  size_t tok_count = mcfg_get_token_count(str);
  if (tok_count < 3) {
    ret.error = MCFG_SYNTAX_ERROR;
    return ret;
  }

  if (type == TYPE_STRING) {
    return _parse_string_field(strchr(str, '\'') + 1);
  }

  if (type == TYPE_LIST) {
    return _parse_list_field(str);
  }

  char *value = mcfg_get_token_raw(str, 2);
  ret = _parse_number_type_field(type, value);
  free(value);

  return ret;
}

mcfg_err_t _parse_outside_sector(char *line, mcfg_parser_ctxt_t *ctxt) {
  mcfg_token_t tok = mcfg_get_token(line, 0);

  if (tok == TOKEN_INVALID)
    return MCFG_INVALID_KEYWORD;

  if (tok == TOKEN_EMPTY || tok == TOKEN_COMMENT)
    return MCFG_OK;

  if (tok == TOKEN_END)
    return MCFG_END_IN_NOWHERE;

  if (tok != TOKEN_SECTOR)
    return MCFG_STRUCTURE_ERROR;

  char *name = mcfg_get_token_raw(line, 1);
  mcfg_err_t ret = mcfg_add_sector(ctxt->target_file, name);

  if (ret != MCFG_OK) {
    free(name);
    return ret;
  }

  ctxt->target_sector =
      &ctxt->target_file->sectors[ctxt->target_file->sector_count - 1];

  return MCFG_OK;
}

mcfg_err_t _parse_sector(char *line, mcfg_parser_ctxt_t *ctxt) {
  mcfg_token_t tok = mcfg_get_token(line, 0);

  if (tok == TOKEN_INVALID)
    return MCFG_INVALID_KEYWORD;

  if (tok == TOKEN_EMPTY || tok == TOKEN_COMMENT)
    return MCFG_OK;

  if (tok == TOKEN_END) {
    ctxt->target_sector = NULL;
    return MCFG_OK;
  }

  if (tok != TOKEN_SECTION)
    return MCFG_STRUCTURE_ERROR;

  if (ctxt->target_sector == NULL)
    return MCFG_STRUCTURE_ERROR;

  char *name = mcfg_get_token_raw(line, 1);
  mcfg_err_t ret = mcfg_add_section(ctxt->target_sector, name);

  if (ret != MCFG_OK) {
    free(name);
    return ret;
  }

  ctxt->target_section =
      &ctxt->target_sector->sections[ctxt->target_sector->section_count - 1];

  return MCFG_OK;
}

mcfg_err_t _parse_section(char *line, mcfg_parser_ctxt_t *ctxt) {
  mcfg_token_t tok = mcfg_get_token(line, 0);

  if (tok == TOKEN_INVALID)
    return MCFG_INVALID_KEYWORD;

  if (tok == TOKEN_EMPTY || tok == TOKEN_COMMENT)
    return MCFG_OK;

  if (tok == TOKEN_END) {
    ctxt->target_section = NULL;
    return MCFG_OK;
  }

  if (tok < TOKEN_STR)
    return MCFG_STRUCTURE_ERROR;

  char *strtype = mcfg_get_token_raw(line, 0);
  mcfg_field_type_t type = mcfg_str_to_type(strtype);
  free(strtype);

  uint16_t name_tok_offs = type == TYPE_LIST ? 1 : 0;
  char *name = mcfg_get_token_raw(line, 1 + name_tok_offs);
  mcfg_data_parse_result_t data_result = mcfg_parse_field_data(type, line);
  if (data_result.error != MCFG_OK)
    return data_result.error;

  mcfg_err_t ret = mcfg_add_field(ctxt->target_section, type, name,
                                  data_result.data, data_result.size);
  if (ret != MCFG_OK) {
    free(name);
    if (data_result.data != NULL)
      free(data_result.data);
    return ret;
  }

  if (data_result.multiline)
    ctxt->target_field =
        &ctxt->target_section->fields[ctxt->target_section->field_count - 1];

  return MCFG_OK;
}

mcfg_err_t _parse_field(char *line, mcfg_parser_ctxt_t *ctxt) {
  if (ctxt->target_field == NULL)
    return MCFG_INVALID_PARSER_STATE;

  mcfg_field_t *field = ctxt->target_field;
  if (field->type != TYPE_STRING && field->type != TYPE_LIST)
    return MCFG_INVALID_PARSER_STATE;

  mcfg_data_parse_result_t data_result;
  switch (field->type) {
  case TYPE_STRING: {
    data_result = _parse_string_field(line);
    if (data_result.error != MCFG_OK)
      return data_result.error;

    size_t new_size = field->size + data_result.size - 1;
    char *new_str = malloc_or_die(new_size);

    // string field data is always expected to be NULL-terminated
    memcpy(new_str, field->data, field->size - 1);
    memcpy(new_str + field->size - 1, data_result.data, data_result.size);
    free(field->data);
    free(data_result.data);

    field->size = new_size;
    field->data = (void *)new_str;
    break;
  }
  case TYPE_LIST: {
    data_result = _parse_list_data((mcfg_list_t *)field->data,
                                   line + _token_position(line, 0));
    break;
  }
  default:
    return MCFG_INVALID_PARSER_STATE;
  }

  if (!data_result.multiline)
    ctxt->target_field = NULL;

  return MCFG_OK;
}

mcfg_err_t mcfg_parse_line(char *line, mcfg_parser_ctxt_t *ctxt) {
  if (ctxt->target_file == NULL)
    return MCFG_INVALID_PARSER_STATE;

  if (ctxt->target_sector == NULL)
    return _parse_outside_sector(line, ctxt);

  if (ctxt->target_section == NULL)
    return _parse_sector(line, ctxt);

  if (ctxt->target_field == NULL)
    return _parse_section(line, ctxt);
  else
    return _parse_field(line, ctxt);

  return MCFG_INVALID_PARSER_STATE;
}

mcfg_err_t mcfg_parse_file_ctxto(char *path, mcfg_file_t *file,
                                 mcfg_parser_ctxt_t **ctxt_out) {
  FILE *in_file;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  file->sector_count = 0;
  file->dynfield_count = 0;

  mcfg_parser_ctxt_t ctxt = {
      .target_file = file,
      .target_sector = NULL,
      .target_section = NULL,
      .target_field = NULL,
      .linenum = 0,
      .file_path = path,
  };

  if (ctxt_out != NULL)
    *ctxt_out = &ctxt;

  errno = 0;
  in_file = fopen(path, "r");
  if (in_file == NULL)
    return MCFG_OS_ERROR_MASK | errno;

  mcfg_err_t result = MCFG_OK;
  while ((read = getline(&line, &len, in_file)) != -1) {
    ctxt.linenum++;
    result = mcfg_parse_line(line, &ctxt);
    if (result != MCFG_OK)
      break;
  }

  fclose(in_file);
  if (line)
    free(line);

  return result;
}

mcfg_err_t mcfg_parse_file(char *path, mcfg_file_t *file) {
  return mcfg_parse_file_ctxto(path, file, NULL);
}

mcfg_err_t mcfg_add_sector(mcfg_file_t *file, char *name) {
  if (file == NULL)
    return MCFG_NULLPTR;

  size_t ix = file->sector_count;

  remove_newline(name);
  if (file->sector_count == 0) {
    file->sectors = malloc_or_die(sizeof(*file->sectors));
  } else {
    if (mcfg_get_sector(file, name) != NULL)
      return MCFG_DUPLICATE_SECTOR;
    file->sectors = realloc_or_die(file->sectors, sizeof(mcfg_sector_t) *
                                                      (file->sector_count + 1));
  }

  file->sectors[ix].name = name;
  file->sectors[ix].section_count = 0;
  file->sector_count++;
  return MCFG_OK;
}

mcfg_err_t mcfg_add_section(mcfg_sector_t *sector, char *name) {
  if (sector == NULL)
    return MCFG_NULLPTR;

  size_t ix = sector->section_count;

  remove_newline(name);
  if (sector->section_count == 0) {
    sector->sections = malloc_or_die(sizeof(*sector->sections));
  } else {
    if (mcfg_get_section(sector, name) != NULL)
      return MCFG_DUPLICATE_FIELD;
    sector->sections = realloc_or_die(
        sector->sections, sizeof(mcfg_section_t) * (sector->section_count + 1));
  }

  sector->sections[ix].name = name;
  sector->sections[ix].field_count = 0;
  sector->section_count++;
  return MCFG_OK;
}

mcfg_err_t mcfg_add_dynfield(mcfg_file_t *file, mcfg_field_type_t type,
                             char *name, void *data, size_t size) {
  if (file == NULL)
    return MCFG_NULLPTR;

  size_t ix = file->dynfield_count;

  remove_newline(name);
  if (file->dynfield_count == 0) {
    file->dynfields = malloc_or_die(sizeof(*file->dynfields));
  } else {
    if (mcfg_get_dynfield(file, name) != NULL)
      return MCFG_DUPLICATE_DYNFIELD;
    file->dynfields = realloc_or_die(
        file->dynfields, sizeof(*file->dynfields) * (file->dynfield_count + 1));
  }

  file->dynfields[ix].type = type;
  file->dynfields[ix].name = name;
  file->dynfields[ix].data = data;
  file->dynfields[ix].size = size;
  file->dynfield_count++;
  return MCFG_OK;
}

mcfg_err_t mcfg_add_field(mcfg_section_t *section, mcfg_field_type_t type,
                          char *name, void *data, size_t size) {
  if (section == NULL)
    return MCFG_NULLPTR;

  size_t ix = section->field_count;

  remove_newline(name);
  if (section->field_count == 0) {
    section->fields = malloc_or_die(sizeof(*section->fields));
  } else {
    if (mcfg_get_field(section, name) != NULL)
      return MCFG_DUPLICATE_FIELD;
    section->fields = realloc_or_die(
        section->fields, sizeof(mcfg_field_t) * (section->field_count + 1));
  }

  section->fields[ix].type = type;
  section->fields[ix].name = name;
  section->fields[ix].data = data;
  section->fields[ix].size = size;
  section->field_count++;
  return MCFG_OK;
}

mcfg_err_t mcfg_add_list_field(mcfg_list_t *list, size_t size, void *data) {
  if (list == NULL || data == NULL)
    return MCFG_NULLPTR;

  char *name = malloc((size_t)floor(log10((double)SIZE_MAX)));
  sprintf(name, "%zu", list->field_count + 1);

  size_t ix = list->field_count;

  if (list->field_count == 0) {
    list->fields = malloc_or_die(sizeof(*list->fields));
  } else {
    list->fields = realloc_or_die(list->fields, sizeof(mcfg_field_t) *
                                                    (list->field_count + 1));
  }

  list->fields[ix].type = list->type;
  list->fields[ix].name = name;
  list->fields[ix].data = data;
  list->fields[ix].size = size;
  list->field_count++;
  return MCFG_OK;
}

mcfg_sector_t *mcfg_get_sector(mcfg_file_t *file, char *name) {
  mcfg_sector_t *ret = NULL;

  for (size_t ix = 0; ix < file->sector_count; ix++) {
    if (strcmp(file->sectors[ix].name, name) == 0) {
      ret = &file->sectors[ix];
      break;
    }
  }

  return ret;
}

mcfg_section_t *mcfg_get_section(mcfg_sector_t *sector, char *name) {
  mcfg_section_t *ret = NULL;

  for (size_t ix = 0; ix < sector->section_count; ix++) {
    if (strcmp(sector->sections[ix].name, name) == 0) {
      ret = &sector->sections[ix];
      break;
    }
  }

  return ret;
}

mcfg_field_t *mcfg_get_dynfield(mcfg_file_t *file, char *name) {
  mcfg_field_t *ret = NULL;

  for (size_t ix = 0; ix < file->dynfield_count; ix++) {
    if (strcmp(file->dynfields[ix].name, name) == 0) {
      ret = &file->dynfields[ix];
      break;
    }
  }

  return ret;
}

mcfg_field_t *mcfg_get_field(mcfg_section_t *section, char *name) {
  mcfg_field_t *ret = NULL;

  for (size_t ix = 0; ix < section->field_count; ix++) {
    if (strcmp(section->fields[ix].name, name) == 0) {
      ret = &section->fields[ix];
      break;
    }
  }

  return ret;
}

void mcfg_free_list(mcfg_list_t *list) {
  if (list == NULL)
    return;

  for (size_t ix = 0; ix < list->field_count; ix++)
    mcfg_free_field(&list->fields[ix]);

  free(list->fields);
}

void mcfg_free_field(mcfg_field_t *field) {
  if (field == NULL)
    return;

  if (field->name != NULL)
    free(field->name);

  if (field->data != NULL) {
    if (field->type == TYPE_LIST)
      mcfg_free_list((mcfg_list_t *)field->data);

    free(field->data);
  }
}

void mcfg_free_section(mcfg_section_t *section) {
  if (section == NULL)
    return;

  if (section->field_count > 0 && section->fields != NULL)
    for (size_t ix = 0; ix < section->field_count; ix++)
      mcfg_free_field(&section->fields[ix]);

  if (section->fields != NULL)
    free(section->fields);

  if (section->name != NULL)
    free(section->name);
}

void mcfg_free_sector(mcfg_sector_t *sector) {
  if (sector == NULL)
    return;

  if (sector->section_count > 0 && sector->sections != NULL)
    for (size_t ix = 0; ix < sector->section_count; ix++)
      mcfg_free_section(&sector->sections[ix]);

  if (sector->sections != NULL)
    free(sector->sections);

  if (sector->name != NULL)
    free(sector->name);
}

void mcfg_free_file(mcfg_file_t *file) {
  if (file == NULL)
    return;

  if (file->dynfield_count > 0 && file->dynfields != NULL)
    for (size_t ix = 0; ix < file->dynfield_count; ix++)
      mcfg_free_field(&file->dynfields[ix]);

  if (file->dynfields != NULL)
    free(file->dynfields);

  if (file->sector_count > 0 && file->sectors != NULL)
    for (size_t ix = 0; ix < file->sector_count; ix++)
      mcfg_free_sector(&file->sectors[ix]);

  if (file->sectors != NULL)
    free(file->sectors);

  free(file);
}
