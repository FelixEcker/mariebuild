// build.c ; mariebuild build functionality impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "build.h"

#include <stdlib.h>

#include "mcfg.h"
#include "mcfg_util.h"

#include "logging.h"
#include "target.h"
#include "types.h"
#include "xmem.h"

config_t default_config = {
    .build_type = BUILD_TYPE_INCREMENTAL,
    .default_target = "debug",
    .target = NULL,
    .public_targets = {.capacity = 0, .item_count = 0, .items = NULL},
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

  if (mcfg_get_sector(file, "targets") == NULL) {
    mb_log(LOG_ERROR, "no targets defined!\n");
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
    mcfg_list_t targets = *mcfg_data_as_list(*field_targets);
    ret.public_targets = strlist_new(targets.field_count, false);

    if (targets.type != TYPE_STRING) {
      mb_log(LOG_WARNING, "mariebuild target list is not a string-list!");
    }

    for (size_t ix = 0; ix < targets.field_count; ix++) {
      char *str = mcfg_data_as_string(targets.fields[ix]);
      if (str != NULL)
        strlist_append(&ret.public_targets, str);
    }

    mb_logf(LOG_DEBUG, "registered %zu public targets\n",
            ret.public_targets.item_count);

    strlist_destroy(&fallback.public_targets);
  } else {
    ret.public_targets = fallback.public_targets;
  }

  mcfg_field_t *field_default_target = mcfg_get_field(config, "default");
  if (field_default_target != NULL) {
    ret.default_target = mcfg_data_as_string(*field_default_target);
  } else {
    ret.default_target = fallback.default_target;
  }

  mcfg_field_t *field_build_type = mcfg_get_field(config, "build_type");
  if (field_build_type != NULL) {
    ret.build_type = str_to_build_type(mcfg_data_as_string(*field_build_type),
                                       fallback.build_type);
  } else {
    ret.build_type = fallback.build_type;
  }

  return ret;
}

int mb_start(args_t args) {
  default_config.public_targets = strlist_new(1, false);
  strlist_append(&default_config.public_targets, "debug");

  int return_code = 0;

  mb_log(LOG_DEBUG, "using MCFG/2 " MCFG_2_VERSION "\n");

  mcfg_file_t *file = xmalloc(sizeof(mcfg_file_t));
  mcfg_parser_ctxt_t *ctxt;
  mcfg_err_t ret = mcfg_parse_file_ctxto(args.buildfile, file, &ctxt);
  if (ret != MCFG_OK) {
    mb_logf(LOG_ERROR, "buildfile parsing failed: %s (%d)\n",
            mcfg_err_string(ret), ret);
    mb_logf(LOG_ERROR, "in file \"%s\" on line %d\n", args.buildfile,
            ctxt->linenum);

    return_code = 1;
    goto exit;
  }

  if (!check_file_validity(file))
    goto exit;

  config_t cfg = mb_load_configuration(*file);
  cfg.target = args.target == NULL ? cfg.default_target : args.target;
  cfg.ignore_failures = args.keep_going;
  cfg.always_force = args.force;

  return_code = mb_begin_build(file, cfg);
  if (return_code != 0)
    mb_log(LOG_ERROR, "build failed!\n");
  else
    mb_log(LOG_INFO, "build succeeded!\n");

exit:
  strlist_destroy(&cfg.public_targets);
  mcfg_free_file(file);
  return return_code;
}

int mb_begin_build(mcfg_file_t *file, config_t cfg) {
  mcfg_sector_t *targets = mcfg_get_sector(file, "targets");
  if (targets == NULL || targets->section_count == 0) {
    mb_log(LOG_ERROR, "build file is missing target definitions!\n");
    return 1;
  }

  mcfg_section_t *target = mcfg_get_section(targets, cfg.target);

  if (target == NULL) {
    mb_logf(LOG_ERROR, "target \"%s\" is not declared within build file!\n",
            cfg.target);
    return 1;
  }

  strlist_t history = strlist_new(1, true);
  int ret = mb_run_target(file, target, &history, cfg);
  strlist_destroy(&history);

  return ret;
}
