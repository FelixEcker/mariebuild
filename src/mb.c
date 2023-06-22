#include <stdlib.h>
#include <stdio.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>
#include <mariebuild/mb_utils.h>

int main() {
  struct mb_file* build_file = malloc(sizeof(mb_file));
  build_file->path = "./build.mb";
  int result = parse_file(build_file);

  if ((result & MB_SERR_MASK_ERRNO) == MB_SERR_MASK_ERRNO)
    result = result & 0;

  if (result != 0) {
    // The error message and code have to be split since mb_log only accepts
    // strings for formatting at the moment.
    mb_log(MB_LOGLVL_IMP, "Parsing failed: Line ", "");
    printf("%d\n", build_file->line);
    mb_log(MB_LOGLVL_IMP, "Parsing failed: %s ", errcode_msg(result));
    printf("(0x%.8x)\n", result);
    mb_log(MB_LOGLVL_IMP, "Aborting build...\n", "");

    free_build_file(build_file);
    return result;
  }

  free_build_file(build_file);
  return 0;
}
