// target.c ; mariebuild build target impl.
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "target.h"

#include <string.h>

#include "mcfg.h"
#include "mcfg_util.h"

#include "c_rule.h"
#include "executor.h"
#include "logging.h"
#include "strlist.h"
#include "types.h"
#include "xmem.h"

int run_required_targets(mcfg_file_t *file, mcfg_section_t *target,
                         strlist_t *target_history, const config_t cfg) {
  mcfg_field_t *field_required_targets =
      mcfg_get_field(target, "required_targets");
  if (field_required_targets == NULL)
    return 0;

  mcfg_list_t *required_targets = mcfg_data_as_list(*field_required_targets);
  mcfg_sector_t *targets = mcfg_get_sector(file, "targets");

  for (size_t ix = 0; ix < required_targets->field_count; ix++) {
    char *curr_target_name = mcfg_data_to_string(required_targets->fields[ix]);
    if (curr_target_name == NULL) {
      mb_logf(LOG_WARNING, "%s/%s:%d: curr_target_name is NULL!\n", __FILE__,
              __FUNCTION__, __LINE__);
      continue;
    }

    mcfg_section_t *curr_target = mcfg_get_section(targets, curr_target_name);
    if (curr_target == NULL) {
      mb_logf(LOG_ERROR,
              "target \"%s\" required by target \"%s\" does not exist.\n",
              curr_target_name, target->name);
      xfree(curr_target_name);

      if (cfg.ignore_failures)
        continue;

      return 1;
    }

    int ret = mb_run_target(file, curr_target, target_history, cfg);
    xfree(curr_target_name);

    if (ret != 0 && !cfg.ignore_failures)
      return ret;
  }

  return 0;
}

int mb_run_target(mcfg_file_t *file, mcfg_section_t *target,
                  strlist_t *target_history, const config_t cfg) {
  if (target_history == NULL) {
    mb_log(LOG_ERROR,
           "internal: mb_run_target was passed a NULL target_history!\n");
    return 1;
  }

  if (strlist_contains_value(target_history, target->name) != -1) {
    mb_logf(LOG_ERROR, "circular target dependency for target \"%s\"\n",
            target->name);
    mb_log(LOG_ERROR, "target history:\n");
    for (size_t ix = 0; ix < target_history->item_count; ix++) {
      mb_logf(LOG_ERROR, "  %s\n", strlist_get(target_history, ix));
    }
    return 1;
  }

  strlist_append(target_history, strdup(target->name));

  mb_logf(LOG_INFO, "building target \"%s\"\n", target->name);

  // Target Execution Order
  // 1. required targets
  // 2. compilation rules
  // 3. "exec" field

  int ret;
  ret = run_required_targets(file, target, target_history, cfg);
  if (ret != 0)
    return ret;

  ret = mb_run_c_rules(file, mcfg_get_field(target, "c_rules"), TARGET,
                       target->name, cfg);
  if (ret != 0)
    return ret;

  mcfg_field_t *field_exec = mcfg_get_field(target, "exec");
  char *exec = NULL;
  if (field_exec != NULL) {
    char *raw_exec = mcfg_data_to_string(*field_exec);
    mcfg_path_t pathrel = {.absolute = true,
                           .dynfield_path = false,

                           .sector = "targets",
                           .section = target->name,
                           .field = ""};
    exec = mcfg_format_field_embeds_str(raw_exec, *file, pathrel);
    xfree(raw_exec);
  }

  if (exec != NULL) {
    ret = mb_exec(exec, target->name);
    xfree(exec);
    if (ret != 0 && !cfg.ignore_failures)
      return ret;
  }

  mb_logf(LOG_INFO, "built target \"%s\"!\n", target->name);
  int ix = strlist_contains_value(target_history, target->name);
  if (ix > -1)
    xfree(target_history->items[ix]);
  target_history->item_count--;
  return 0;
}
