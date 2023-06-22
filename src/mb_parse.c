/*
 * mariebuild: mb_parse.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <mariebuild/mb_parse.h>

#include <mariebuild/mb_utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

/******** Local Utility Functions ********/

/* Note: This function returns a pointer to a substring of the original string.
 * If the given string was allocated dynamically, the caller must not overwrite
 * that pointer with the returned value, since the original pointer must be
 * deallocated using the same allocator with which it was allocated.  The return
 * value must NOT be deallocated using free() etc.
 * */
char *trim_whitespace(char *str) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

/******** mb_parse.h ********/

void free_build_file(mb_file* file) {
  for (int i = 0; i < file->sector_count; i++) {
    for (int j = 0; j < file->sectors[i].section_count; j++) {
      for (int k = 0; k < file->sectors[i].sections[j].field_count; k++) {
        free(file->sectors[i].sections[j].fields[k].name);
        free(file->sectors[i].sections[j].fields[k].value);
      }

      free(file->sectors[i].sections[j].fields);
      free(file->sectors[i].sections[j].name);
      free(file->sectors[i].sections[j].lines);
    }

    free(file->sectors[i].sections);
    free(file->sectors[i].name);
  }

  free(file->sectors);
  free(file);
}

int register_sector(struct mb_file* file, char *name) {
  if (name == NULL) return MB_ERR_UNKNOWN;

  // Check for duplicate sectors
  for (int i = 0; i < file->sector_count; i++)
    if (strcmp(file->sectors[i].name, name) == 0)
      return MB_PERR_DUPLICATE_SECTOR;

  int wi = file->sector_count;
  file->sector_count++;

  if (wi == 0) {
    file->sectors = malloc(sizeof(mb_sector));
  } else {
    file->sectors = realloc(file->sectors, (wi+1) * sizeof(mb_sector));
  }

  file->sectors[wi].section_count = 0;
  file->sectors[wi].name = malloc(strlen(name) + 1);
  strcpy(file->sectors[wi].name, name);

  mb_log(MB_LOGLVL_LOW, "registered sector %s\n", name);
  return MB_OK;
}

int register_section(struct mb_sector* sector, char *name) {
  if (name == NULL) return MB_ERR_UNKNOWN;

  // Check for duplicate sections
  for (int i = 0; i < sector->section_count; i++)
    if (strcmp(sector->sections[i].name, name) == 0)
      return MB_PERR_DUPLICATE_SECTION;

  int wi = sector->section_count;
  sector->section_count++;

  if (wi == 0) {
    sector->sections = malloc(sizeof(mb_section));
  } else {
    sector->sections = realloc(sector->sections, (wi+1) * sizeof(mb_section));
  }

  sector->sections[wi].field_count = 0;
  sector->sections[wi].name = malloc(strlen(name) + 1);
  strcpy(sector->sections[wi].name, name);

  mb_log(MB_LOGLVL_LOW, "registered section %s\n", name);
  return MB_OK;
}

int register_field(struct mb_section* section, char *name, char *value) {
  if (name == NULL || value == NULL) return MB_ERR_UNKNOWN;

  // Check for duplicate fields
  for (int i = 0; i < section->field_count; i++)
    if (strcmp(section->fields[i].name, name) == 0)
      return MB_PERR_DUPLICATE_FIELD;

  int wi = section->field_count;
  section->field_count++;

  if (wi == 0) {
    section->fields = malloc(sizeof(mb_field));
  } else {
    section->fields = realloc(section->fields, (wi+1) * sizeof(mb_field));
  }

  section->fields[wi].name = malloc(strlen(name) + 1);
  strcpy(section->fields[wi].name, name);
  section->fields[wi].value = malloc(strlen(value) + 1);
  strcpy(section->fields[wi].value, value);

  mb_log(MB_LOGLVL_LOW, "registered field %s\n", name);
  return MB_OK;
}

/* Parses a single line.
 * */
int parse_line(struct mb_file* file, char *line) {
  char delimiter[] = " ";
  line = trim_whitespace(line);
  char *token = strtok(line, delimiter);

  while (token != NULL) {
    if (strcmp(token, "sector") == 0) {
      token = strtok(NULL, delimiter);

      return register_sector(file, token);
    } else if (strncmp(";", token, strlen(";")) == 0) break;

    if (str_endswith(token, ":") == 1)
      register_section(&file->sectors[file->sector_count-1], token);

    if (file->sector_count == 0)
      return MB_PERR_INVALID_SYNTAX;

    if (file->sectors[file->sector_count-1].section_count == 0)
      return MB_PERR_INVALID_SYNTAX;

    mb_sector sector = file->sectors[file->sector_count-1];
    mb_section section = sector.sections[sector.section_count-1];

    token = strtok(NULL, delimiter);
  }

  return MB_OK;
}

/* Parses the file under the path in build_file->path line by line,
 * will return MB_OK if there were no errors.
 *
 * Refer to mb_parse.h for error codes.
 * */
int parse_file(struct mb_file* build_file) {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  errno = 0;
  file = fopen(build_file->path, "r");
  if (file == NULL)
    return MB_SERR_MASK_ERRNO | errno;

  build_file->sector_count = 0;
  build_file->line = 0;

  while ((read = getline(&line, &len, file)) != -1) {
    build_file->line++;
    int result = parse_line(build_file, line);
    if (result != MB_OK)
      return result;
  }

  fclose(file);
  if (line)
    free(line);

  return MB_OK;
}
