#include <mb_extensions.h>

#include <mb_utils.h>

#include <butter/fsutils.h>

#include <stdlib.h>
#include <stdio.h>

struct mb_ext_reg *mb_ext_register = NULL;

int mb_register_extensions(mcfg_file build_file, struct mb_ext_reg *registry) {
  char *ext_home = getenv(MB_EXT_HOME);
  if (ext_home == NULL) {
    mb_log(MB_LOGLVL_IMP, "environment variable MBEXT_HOME is unset; "
                          "assuming default of ~/.mbext/\n");
    ext_home = "~/.mbext/";
  }

  char *expanded = expand_path(ext_home);
  printf("%s\n", expanded);
  free(expanded);
  return MB_OK;
}

int mb_load_extensions(struct mb_ext_reg *registry) {
  return MB_OK;
}

int mb_execute_extension_script(mb_ext_reg *registry, char *extension_name, 
                                char* script_name) {
  return MB_OK;
}
