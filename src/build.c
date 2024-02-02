// build.c ; mariebuild build functionality impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "build.h"

#include <stdlib.h>

#include "mcfg.h"
#include "mcfg_util.h"

#include "xmem.h"
#include "logging.h"

config_t default_config = {
  .default_target = "debug",
  .target = NULL,
  .public_targets = { .capacity = 0, .item_count = 0, .items = NULL },
  .always_force = false,
  .ignore_failures = false,
};

bool check_file_validity(mcfg_file_t *file) {
  if (file == NULL) {
    mb_log(LOG_ERROR, "internal: build file is NULL!\n");
    return false;
  }

  if (file->sector_count == 0) {
    mb_log(LOG_ERROR, "build file is empty!\n");
    return false;
  }

  return true;
}

config_t mb_load_configuration(mcfg_file_t file) {
  config_t fallback = default_config;
  config_t ret;

  mcfg_sector_t *sector = mcfg_get_sector(&file, "config");
  if (sector == NULL)
    return fallback;

  mcfg_section_t *config = mcfg_get_section(sector, "mariebuild");
  if (config == NULL)
    return fallback;

  mcfg_field_t *field_targets = mcfg_get_field(config, "targets");
  if (field_targets != NULL) {
    mcfg_list_t targets = mcfg_data_as_list(*field_targets);
    ret.public_targets = strlist_init(targets.field_count);

    if (targets.type != TYPE_STRING) {
      mb_log(LOG_WARNING, "mariebuild target list is not a string-list!");
    }
  }

  return ret;
}

int mb_start(args_t args) {
  default_config.public_targets = strlist_new(1);
  strlist_append(&default_config.public_targets, "debug");

  int return_code = 0;

  mb_log(LOG_DEBUG, "using MCFG/2 " MCFG_2_VERSION "\n");

  mcfg_file_t *file = xmalloc(sizeof(mcfg_file_t));
  mcfg_parser_ctxt_t *ctxt;
  mcfg_err_t ret = mcfg_parse_file_ctxto(args.buildfile, file, &ctxt);
  if (ret != MCFG_OK) {
    mb_logf(LOG_ERROR, "buildfile parsing failed: %s (%d)\n", mcfg_err_string(ret),
            ret);
    mb_logf(LOG_ERROR, "in file \"%s\" on line %d\n", args.buildfile,
            ctxt->linenum);

    return_code = 1;
    goto exit;
  }

  if (!check_file_validity(file))
    goto exit;

  cfg = mb_load_configuration(*file);
  cfg.target = args.target == NULL ? cfg.default_target : args.target;

  return_code = mb_begin_build(file, cfg);
  if (return_code != 0)
    mb_log(LOG_ERROR, "build failed!\n");

exit:
  mcfg_free_file(file);
  return return_code;
}

int mb_begin_build(mcfg_file_t *file, config_t cfg) {
  mcfg_sector_t *targets = mcfg_get_sector(file, "targets");
  if (targets == NULL || targets->section_count == 0) {
    mb_log(LOG_ERROR, "build file is missing target definitions!\n");
    return 1;
  }

  build_state_t state = {
    .target = mcfg_get_section(targets, cfg.target),
    .current_target = NULL,
    .current_rule = NULL,
  };

  if (state.target == NULL) {
    mb_logf(LOG_ERROR, "target \"%s\" is not declared within build file!\n", 
            cfg.target);
    return 1;
  }

  return 0;
}
