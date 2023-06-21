#ifndef MB_PARSE_H
#define MB_PARSE_H

#define MB_OK          0x00000000
#define MB_ERR_UNKNOWN 0x00000001

/* Parsing Errors */
#define MB_PERR_MISSING_REQUIRED   0x00000101
#define MB_PERR_DUPLICATE_SECTION  0x00000102
#define MB_PERR_INVALID_IDENTIFIER 0x00000103
#define MB_PERR_INVALID_SYNTAX     0x00000104

/* Mask if a system error occured which reports its error in errno */
#define MB_SERR_MASK_ERRNO       0x10000000

typedef struct mb_field {
  char *name;
  char value[];
} mb_field;

typedef struct mb_section {
  char     *name;
  int      section_type;
  char     *lines;
  mb_field fields[];
} mb_section;

typedef struct mb_sector {
  char       *name;
  mb_section sections[];
} mb_sector;

typedef struct mb_file {
  char      *path;
  mb_sector sectors[];
} mb_file;

int register_sector(struct mb_file* file, char *name);

int parse_line(struct mb_file* file, char *line);
int parse_file(struct mb_file* file);

#endif
