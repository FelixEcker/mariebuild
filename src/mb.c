#include <stdio.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>

int main() {
  struct mb_file build_file;
  build_file.path = "./build.mb";
  int result = parse_file(&build_file);
  printf("result: %d\n", result);
  return 0;
}
