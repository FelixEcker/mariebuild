#include <mariebuild/mb_parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
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

int register_sector(struct mb_file* file, char *name) {
  return MB_OK;
}

/* Parses a single line. 
 * */
int parse_line(struct mb_file* file, char *line) {
  char delimiter[] = " ";
  line = trim_whitespace(line);
  char *token = strtok(line, delimiter);

  while (token != NULL) {
    if (strcmp(token, "sector")) {
      int result = register_sector(file, strtok(NULL, delimiter));
      if (result != MB_OK)
        return result;

      continue;
    }

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
