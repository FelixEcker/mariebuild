#include <mb_extensions.h>

#include <mb_utils.h>

#include <mcfg.h>

#include <butter/fsutils.h>
#include <butter/strutils.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct mb_ext_reg *mb_ext_register = NULL;

/* Utility Functions */

void mb_free_ext_reg(struct mb_ext_reg *registry) {
  for (int i = 0; i < registry->extension_count; i++) {
    if (registry->extensions[i].file != NULL)
      free_mcfg_file(registry->extensions[i].file);
    if (registry->extensions[i].name != NULL)
      free(registry->extensions[i].name);
    if (registry->extensions[i].path != NULL)
      free(registry->extensions[i].path);
  }
  free(registry->extensions);
  free(registry);
}

/* Registry & Loading Functions */

static int _reg_ext(struct mb_ext_reg *registry, char *name, char *path) {
  if (name == NULL || path == NULL) {
    mb_errtext = "Encountered nullptr in static int mb_extensions::_reg_ext";
    return MB_GERR_NULLPTR;
  }

  int ix = registry->extension_count;
  registry->extension_count++;
  if (registry->extension_count == 1) {
    registry->extensions = malloc(sizeof(mb_extension));
  } else {
    registry->extensions = realloc(registry->extensions, 
                            sizeof(mb_extension) * registry->extension_count);
  }

  registry->extensions[ix].name = name;
  registry->extensions[ix].path = path;
  return MB_OK;
}

int mb_register_extensions(mcfg_file build_file, struct mb_ext_reg *registry) {
  mcfg_section *extlist = find_section(&build_file, ".config/extensions");
  if (extlist == NULL) {
    mb_log(MB_LOGLVL_LOW, "section .config/extensions does not exist, aborting!"
                          "\n");
    return MB_OK;
  }

  if (extlist->type != ST_LINES) {
    mb_log(MB_LOGLVL_LOW, "section .config/extensions is not of type lines, "
                          "aborting!\n");
    return MB_OK;
  }

  char *ext_home = getenv(MB_EXT_HOME);
  if (ext_home == NULL) {
    mb_log(MB_LOGLVL_IMP, "environment variable MBEXT_HOME is unset; "
                          "assuming default of ~/.mbext/\n");
    ext_home = "~/.mbext/";
  }

  int ret = MB_OK;

  char *expanded = expand_path(ext_home);
  char *strtok_ctxt;
  char *extlistcpy = strdup(extlist->lines);
  char *extension = strtok_r(extlistcpy, newline, &strtok_ctxt);

  while (extension != NULL) {
    size_t pathsz = strlen(expanded) + strlen(extension) + 2;
    char *path = malloc(pathsz);
    sprintf(path, "%s%s/", expanded, extension);
    
    ret = _reg_ext(registry, strdup(extension), path);
    if (ret != MB_OK) {
      goto mb_register_extension_end;
    }
    extension = strtok_r(NULL, newline, &strtok_ctxt);
  }

mb_register_extension_end:;
  free(extlistcpy);
  free(expanded);
  return ret;
}

int mb_load_extensions(struct mb_ext_reg *registry) {
  return MB_OK;
}

int mb_execute_extension_script(mb_ext_reg *registry, char *extension_name, 
                                char* script_name) {
  return MB_OK;
}
