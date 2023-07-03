/*
 * mariebuild: mb.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>

#include <mariebuild/mb_parse.h>
#include <mariebuild/mb_execute.h>
#include <mariebuild/mb_utils.h>

const char *argp_program_version = "mariebuild 0.1.0";
const char *argp_program_bug_address = 
  "https://github.com/FelixEcker/mariebuild/issues";
const char description[] = 
  "A simple build system inspired by my hate against makefiles\n"
  "Author: Marie Eckert";
const char args_doc[] = "";

static struct argp_option options[] = {
  {"in", 'i', "FILE", 0, "Specify a buildfile"}
, {"exec", 'e', "SCRIPT", 0, "Specify a specific script to be executed"}
, {"check", 'c', 0, 0, "Check if a build file is valid"}
, {"mode", 'm', "MODE", 0, "Specify the building mode"}
, {"platform", 'p', "PLATFORM", 0, "Specify the targeted platform"}
, {"list-platforms", 'l', 0, 0, 
   "Get a list of platforms supported by the build-file"}
, {"disable-extensions", 'd', 0, 0, 
   "Disable all extensions used by the build-file"}
, {"quiet", 'q', 0, 0, "Disable all output except for Important messages"}
, {"verbose", 'v', 0, 0, "Output all messages"}
, {0, 0, 0, 0}
};

struct arguments {
  char *build_file;
  char *exec_script;
  bool check_file;
  char *mode;
  char *platform;
  bool disable_extensions;
  int  log_level;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *args = state->input;
  switch (key) {
  case 'i': args->build_file = arg; break;
  case 'e': args->exec_script = arg; break;
  case 'c': args->check_file = true; break;
  case 'm': args->mode = arg; break;
  case 'p': args->platform = arg; break;
  case 'd': args->disable_extensions = true; break;
  case 'q': args->log_level = MB_LOGLVL_IMP; break;
  case 'v': args->log_level = MB_LOGLVL_LOW; break;
  default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, description };

void print_structure(struct mb_file* build_file) {
  printf("\n==========================\n\n");
  for (int i = 0; i < build_file->sector_count; i++) {
    mb_sector sector = build_file->sectors[i];
    printf("sector: %s\n", sector.name);
    printf("sections:\n");
    for (int j = 0; j < sector.section_count; j++) {
      mb_section section = sector.sections[j];
      printf("  section: %s\n", section.name);
      printf("  lines: %s\n", section.lines);
      printf("  fields:\n");
      for (int k = 0; k < section.field_count; k++) {
        mb_field field = section.fields[k];
        printf("    name: %s\n", field.name);
        printf("    value: %s\n", field.value);
      }
    }
  }
  printf("\n==========================\n");
}

int main(int argc, char **argv) {
  struct arguments args;
  args.build_file = "./build.mb";
  args.exec_script = NULL;
  args.check_file = false;
  args.mode = "debug";
  args.platform = NULL;
  args.disable_extensions = false;
  args.log_level = MB_LOGLVL_STD;

  argp_parse(&argp, argc, argv, 0, 0, &args);

  mb_logging_level = args.log_level;
  struct mb_file* build_file = malloc(sizeof(mb_file));
  build_file->path = args.build_file;
  int result = parse_file(build_file);

  if ((result & MB_SERR_MASK_ERRNO) == MB_SERR_MASK_ERRNO)
    result = result & 0;

  if (result != 0) {
    mb_logf(MB_LOGLVL_IMP, "Parsing failed: Line %d\n", build_file->line);
    mb_logf(MB_LOGLVL_IMP, "Parsing failed: %s (0x%.8x)\n", errcode_msg(result), 
                                                          result);
    mb_log(MB_LOGLVL_IMP, "Aborting build...\n");

    goto mb_exit;
  }

  if (args.check_file) {
    mb_log(MB_LOGLVL_IMP, "Build-File passed\n");
    goto mb_exit;
  }
  
  struct mb_exec_params exec_params;
  exec_params.exec_script      = args.exec_script;
  exec_params.platform         = args.platform;
  exec_params.mode             = args.mode;
  exec_params.allow_extensions = !args.disable_extensions;

  result = mb_exec_build(build_file, exec_params);
  if (result != 0) {
    mb_logf(MB_LOGLVL_IMP, "Build failed: %s (0x%.8x)\n", errcode_msg(result),
                                                        result);
  } else {
    mb_log(MB_LOGLVL_STD, "Build succeeded!\n");
  }

mb_exit:
  free_build_file(build_file);
  return result;
}
