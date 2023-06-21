#include <mariebuild/mb_parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

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
  printf("registered sector %s\n", name);

  // Check for duplicate sectors
  for (int i = 0; i < file->sector_count; i++) {
    if (strcmp(file->sectors[i].name, name) == 0) {
      return MB_PERR_DUPLICATE_SECTOR;
    }
  }

  int wi = file->sector_count;
  file->sector_count++;

  if (file->sector_count == 1) {
    file->sectors = malloc(sizeof(mb_sector));
  } else {
    file->sectors = realloc(file->sectors, 
                            file->sector_count * sizeof(mb_sector));
  }

  file->sectors[wi].name = malloc(strlen(name) + 1);
  strcpy(file->sectors[wi].name, name);
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

      int result = register_sector(file, token);
      if (result != MB_OK)
        return result; 
    } else if (strncmp(";", token, strlen(";")) == 0) break;

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

  while ((read = getline(&line, &len, file)) != -1) {
    int result = parse_line(build_file, line);
    if (result != MB_OK)
      return result;
  }

  fclose(file);
  if (line)
    free(line);

  return MB_OK;
}
