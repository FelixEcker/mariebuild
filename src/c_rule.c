// c_rule.c ; mariebuild build rule implementation
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#include "c_rule.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "mcfg.h"
#include "mcfg_util.h"

#include "executor.h"
#include "logging.h"
#include "types.h"
#include "xmem.h"

struct io_fields {
  mcfg_field_t *input;
  mcfg_field_t *output;
};

bool is_file_newer(char *file1, char *file2) {
  FILE *f_1 = fopen(file1, "r");
  FILE *f_2 = fopen(file2, "r");

  int f_1_mtime = 0;
  int f_2_mtime = 0;

  if (f_1 == NULL || f_2 == NULL) {
    if (errno != ENOENT) {
      mb_logf(LOG_DEBUG, "file opening for file %d failed: OS Error %d (%s)\n",
              f_1 == NULL ? 1 : 2, errno, strerror(errno));
    }
    goto exit;
  }

  int fd_1 = fileno(f_1);
  int fd_2 = fileno(f_2);

  struct stat f_1_stat;
  if (fstat(fd_1, &f_1_stat) != 0) {
    mb_logf(LOG_ERROR, "%s/%s:%d: fstat for f_1 failed: OS Error %d (%s)\n",
            __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
    goto exit;
  }

  struct stat f_2_stat;
  if (fstat(fd_2, &f_2_stat) != 0) {
    mb_logf(LOG_ERROR, "%s/%s:%d: fstat for f_2 failed: OS Error %d (%s)\n",
            __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
    goto exit;
  }

  f_1_mtime = f_1_stat.st_mtim.tv_sec;
  f_2_mtime = f_2_stat.st_mtim.tv_sec;

exit:
  if (f_1 != NULL)
    fclose(f_1);
  if (f_2 != NULL)
    fclose(f_2);

  return f_1_mtime > f_2_mtime;
}

bool get_io_fields(mcfg_file_t *file, mcfg_section_t *rule,
                   struct io_fields *dest) {
  mcfg_field_t *field_input = mcfg_get_field(rule, "input");
  if (field_input == NULL) {
    mcfg_field_t *field_input_src = mcfg_get_field(rule, "input_src");
    if (field_input_src == NULL) {
      mb_log(LOG_ERROR, "missing input element list!\n");
      return false;
    }

    char *raw_path = mcfg_data_to_string(*field_input_src);
    field_input = mcfg_get_field_by_path(file, mcfg_parse_path(raw_path));

    if (field_input == NULL) {
      xfree(raw_path); // mcfg_parse_path mangles the input

      raw_path = mcfg_data_to_string(*field_input_src);
      mb_logf(LOG_ERROR, "field \"%s\" does not exit!\n", raw_path);
      xfree(raw_path);

      return false;
    }
  }

  if (field_input->type != TYPE_LIST) {
    mb_log(LOG_ERROR, "field \"input\" is not of type list!\n");
    return false;
  }
  if (field_input->data == NULL) {
    mb_log(LOG_ERROR, "field \"input\" has no data!\n");
    return false;
  }

  mcfg_field_t *field_output = mcfg_get_field(rule, "output");
  if (field_output == NULL) {
    mcfg_field_t *field_output_src = mcfg_get_field(rule, "input_src");
    if (field_output_src == NULL) {
      field_output = field_input;
      goto field_out_null_done;
    }

    char *raw_path = mcfg_data_to_string(*field_output_src);
    field_output = mcfg_get_field_by_path(file, mcfg_parse_path(raw_path));

    if (field_output == NULL) {
      xfree(raw_path); // mcfg_parse_path mangles the input

      raw_path = mcfg_data_to_string(*field_output_src);
      mb_logf(LOG_ERROR, "field \"%s\" does not exit!\n", raw_path);
      xfree(raw_path);

      return false;
    }

  field_out_null_done:;
  } else if (field_output->type != TYPE_LIST) {
    mb_log(LOG_ERROR, "field \"output\" is not of type list!\n");
    return false;
  }

  dest->input = field_input;
  dest->output = field_output;
  return true;
}

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

    if (ret != 0 && !cfg.ignore_failures) {
      return ret;
    }
  }

  return 0;
}

int run_singular(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg,
                 build_type_t build_type) {
  mcfg_field_t *field_exec = mcfg_get_field(rule, "exec");
  if (field_exec == NULL || field_exec->data == NULL) {
    mb_log(LOG_ERROR, "c_rule missing field \"exec\"\n");
    return 1;
  }

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

  struct io_fields io_fields;
  if (!get_io_fields(file, rule, &io_fields))
    return 1;

  mcfg_list_t *list_input = mcfg_data_as_list(*io_fields.input);
  mcfg_list_t *list_output = mcfg_data_as_list(*io_fields.output);

  mcfg_path_t pathrel = {.absolute = true,
                         .dynfield_path = false,

                         .sector = "c_rules",
                         .section = rule->name,
                         .field = ""};

  if (mcfg_get_dynfield(file, "element") == NULL) {
    mcfg_err_t err =
        mcfg_add_dynfield(file, TYPE_STRING, strdup("element"), NULL, 0);
    if (err != MCFG_OK) {
      mb_logf(LOG_ERROR, "mcfg_add_dynfield failed: %s (%d)\n",
              mcfg_err_string(err), err);
    }
  }

  if (mcfg_get_dynfield(file, "input") == NULL) {
    mcfg_err_t err =
        mcfg_add_dynfield(file, TYPE_STRING, strdup("input"), NULL, 0);
    if (err != MCFG_OK) {
      mb_logf(LOG_ERROR, "mcfg_add_dynfield failed: %s (%d)\n",
              mcfg_err_string(err), err);
    }
  }

  if (mcfg_get_dynfield(file, "output") == NULL) {
    mcfg_err_t err =
        mcfg_add_dynfield(file, TYPE_STRING, strdup("output"), NULL, 0);
    if (err != MCFG_OK) {
      mb_logf(LOG_ERROR, "mcfg_add_dynfield failed: %s (%d)\n",
              mcfg_err_string(err), err);
    }
  }

  mcfg_field_t *dynfield_element = mcfg_get_dynfield(file, "element");
  mcfg_field_t *dynfield_input = mcfg_get_dynfield(file, "input");
  mcfg_field_t *dynfield_output = mcfg_get_dynfield(file, "output");

  int ret = 0;

  for (size_t ix = 0; ix < list_output->field_count; ix++) {
    char *raw_in = mcfg_data_to_string(list_input->fields[ix]);
    char *raw_out = mcfg_data_to_string(list_output->fields[ix]);

    dynfield_element->data = raw_in;
    dynfield_element->size = strlen(raw_in) + 1;

    char *in = mcfg_format_field_embeds_str(input_format, *file, pathrel);

    dynfield_element->data = raw_out;
    dynfield_element->size = strlen(raw_in) + 1;

    char *out = mcfg_format_field_embeds_str(output_format, *file, pathrel);

    if (build_type == BUILD_TYPE_INCREMENTAL && !is_file_newer(in, out))
      goto build_loop_continue;

    dynfield_output->data = out;
    dynfield_output->size = strlen(out) + 1;
    dynfield_input->data = in;
    dynfield_input->size = strlen(in) + 1;

    char *script = mcfg_format_field_embeds(*field_exec, *file, pathrel);

    fprintf(stderr, "    exec: %s > %s\n", in, out);

    ret = mb_exec(script, rule->name);

    xfree(script);
  build_loop_continue:
    xfree(raw_in);
    xfree(raw_out);
    xfree(in);
    xfree(out);

    if (ret != 0 && !cfg.ignore_failures)
      break;
  }

  // We have to do this to avoid double-frees when running mcfg_free_file at
  // exit in build.c
  dynfield_element->data = NULL;
  dynfield_input->data = NULL;
  dynfield_output->data = NULL;

  return ret;
}

int run_unify(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg,
              build_type_t build_type) {

  return 0;
}

int mb_run_c_rule(mcfg_file_t *file, mcfg_section_t *rule, const config_t cfg) {
  mb_logf(LOG_INFO, "fulfilling c_rule \"%s\"\n", rule->name);

  mcfg_field_t *field_c_rules = mcfg_get_field(rule, "c_rules");
  if (field_c_rules != NULL) {
    if (field_c_rules->type != TYPE_LIST) {
      mb_log(LOG_WARNING, "field c_rules is of incorrect type! ignoring.\n");
    } else {
      int ret = mb_run_c_rules(file, field_c_rules, C_RULE, rule->name, cfg);
      if (ret != 0)
        return ret;
    }
  }

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
  }

  return 1;
}
