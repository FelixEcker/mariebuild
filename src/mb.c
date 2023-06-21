#include <stdlib.h>
#include <stdio.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>

int main() {
  struct mb_file* build_file = malloc(sizeof(mb_file));
  build_file->path = "./build.mb";
  int result = parse_file(build_file);
  //for (int i = 0; i < build_file->sector_count; i++)
  //  free(build_file->sectors[i].name);
  //free(build_file->sectors);
  //free(build_file);
  free_build_file(build_file);
  if ((result & MB_SERR_MASK_ERRNO) == MB_SERR_MASK_ERRNO)
    result = result & 0;
  printf("result: 0x%.8x\n", result);
  return 0;
}
