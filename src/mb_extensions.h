/*
 * mariebuild: mb_extensions.h ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifndef MB_EXTENSIONS_H
#define MB_EXTENSIONS_H

#include <mcfg.h>

#define MB_EXT_HOME "MBEXT_HOME"

typedef struct mb_extension {
  char             *name;
  char             *path;
  struct mcfg_file *file;
} mb_extension;

typedef struct mb_ext_reg {
  int           extension_count;
  mb_extension *extensions;
} mb_ext_reg;

extern struct mb_ext_reg *mb_ext_register;

/* Registers all extensions which are listed in the build file into the
 * provided registry
 */
int mb_register_extensions(mcfg_file build_file, struct mb_ext_reg *registry);

/* Loads all registered extension's manifest files so that scripts from them
 * may be executed.
 */
int mb_load_extensions(struct mb_ext_reg *registry);

/* Execute script "script_name" from extension "extension_name" registered in
 * the provided registry.
 */
int mb_execute_extension_script(mb_ext_reg *registry, char *extension_name, 
                                char* script_name);

#endif
