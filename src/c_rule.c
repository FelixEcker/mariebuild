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
                   int org_type, char *org_name, const config_t cfg) {
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

int run_singular(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg,
                 build_type_t build_type) {
  mcfg_field_t *field_input_format = mcfg_get_field(rule, "input_format");
  mcfg_field_t *field_output_format = mcfg_get_field(rule, "output_format");

  if (field_input_format == NULL || field_output_format == NULL) {
    mb_logf(LOG_ERROR, "c_rule missing field \"%s\"!\n",
            field_input_format == NULL ? "input_format" : "output_format");
    return 1;
  } else if (field_input_format->type != TYPE_STRING ||
             field_output_format->type != TYPE_STRING) {
    mb_logf(LOG_ERROR, "invalid datatype for field \"%s\"! Expected str\n",
            field_input_format->type != TYPE_STRING ? "input_format"
                                                    : "output_format");
    return 1;
  }

  char *input_format = mcfg_data_as_string(*field_input_format);
  char *output_format = mcfg_data_as_string(*field_output_format);

  if (input_format == NULL || output_format == NULL) {
    mb_logf(LOG_ERROR, "field \"%s\" is missing data!\n",
            input_format == NULL ? "input_format" : "output_format");
    return 1;
  }

  return 0;
}

int run_unify(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg,
              build_type_t build_type) {
  return 0;
}

int mb_run_c_rule(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg) {
  mb_logf(LOG_INFO, "fulfilling c_rule \"%s\"\n", rule->name);

  build_type_t build_type = cfg.build_type;
  if (mcfg_get_field(rule, "build_type") != NULL) {
    char *data = mcfg_data_to_string(*mcfg_get_field(rule, "build_type"));

    build_type = str_to_build_type(data, build_type);

    xfree(data);
  }

  exec_mode_t exec_mode = EXEC_MODE_SINGULAR;
  if (mcfg_get_field(rule, "exec_mode") != NULL) {
    char *data = mcfg_data_to_string(*mcfg_get_field(rule, "exec_mode"));

    exec_mode = str_to_exec_mode(data, exec_mode);

    xfree(data);
  }

  switch (exec_mode) {
  case EXEC_MODE_SINGULAR:
    return run_singular(file, rule, cfg, build_type);
  case EXEC_MODE_UNIFY:
    return run_unify(file, rule, cfg, build_type);
    break;
  }

  return 1;
}
