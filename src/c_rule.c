// c_rule.c ; mariebuild build rule implementation
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "c_rule.h"

#include "mcfg.h"
#include "mcfg_util.h"

#include "logging.h"
#include "types.h"
#include "xmem.h"

int mb_run_c_rules(mcfg_file_t *file, mcfg_field_t *field_required_c_rules,
                   int org_type, char *org_name, config_t cfg) {
  if (field_required_c_rules == NULL)
    return 0;

  mcfg_sector_t *c_rules = mcfg_get_sector(file, "c_rules");
  if (c_rules == NULL || c_rules->section_count == 0) {
    mb_log(LOG_ERROR, "No c_rules defined!\n");
    return 1;
  }

  mcfg_list_t *required_c_rules = mcfg_data_as_list(*field_required_c_rules);

  for (size_t ix = 0; ix < required_c_rules->field_count; ix++) {
    char *curr_c_rule_name = mcfg_data_to_string(required_c_rules->fields[ix]);
    if (curr_c_rule_name == NULL) {
      mb_logf(LOG_WARNING, "%s/%s:%d: curr_c_rule_name is NULL!\n", __FILE__,
              __FUNCTION__, __LINE__);
      continue;
    }

    mcfg_section_t *curr_c_rule = mcfg_get_section(c_rules, curr_c_rule_name);
    if (curr_c_rule == NULL) {
      mb_logf(
          LOG_ERROR, "c_rule \"%s\" required by %s \"%s\" does not exist.\n",
          curr_c_rule_name, org_type == TARGET ? "target" : "c_rule", org_name);
      xfree(curr_c_rule_name);

      if (cfg.ignore_failures)
        continue;

      return 1;
    }

    int ret = mb_run_c_rule(file, curr_c_rule, cfg);
    xfree(curr_c_rule_name);

    if (ret != 0 && !cfg.ignore_failures)
      return ret;
  }

  return 0;
}

int mb_run_c_rule(mcfg_file_t *file, mcfg_section_t *rule, config_t cfg) {
  mb_logf(LOG_INFO, "fulfilling c_rule \"%s\"\n", rule->name);
  return 0;
}
