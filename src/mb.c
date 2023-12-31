/*
 * mariebuild: mb.c ; author: Marie Eckert
 *
 * Copyright (c) 2023, Marie Eckert
 * Licensed under the BSD 3-Clause License
 * <https://github.com/FelixEcker/mariebuild/blob/master/LICENSE>
 */

#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <mcfg.h>

#include <mb_execute.h>
#include <mb_utils.h>

/* argp.h argument parsing functionality */

#define VERSION_STRING "0.3.3"
#define VERSION_TYPE "develop"

const char *argp_program_version =
    "mariebuild " VERSION_STRING " (" VERSION_TYPE ")";
const char *argp_program_bug_address =
    "https://github.com/FelixEcker/mariebuild/issues";
const char description[] =
    "A simple build system inspired by my hate against makefiles\n"
    "Author: Marie Eckert";
const char args_doc[] = "";

static struct argp_option options[] = {
    {"in", 'i', "FILE", 0, "Specify a buildfile"},
    {"exec", 'e', "SCRIPT", 0, "Specify a specific script to be executed"},
    {"check", 'c', 0, 0, "Check if a build file is valid"},
    {"mode", 'm', "MODE", 0, "Specify the building mode"},
    {"platform", 'p', "PLATFORM", 0, "Specify the targeted platform"},
    {"list-platforms", 'l', 0, 0,
     "Get a list of platforms supported by the build-file"},
    {"disable-extensions", 'd', 0, 0,
     "Disable all extensions used by the build-file"},
    {"quiet", 'q', 0, 0, "Disable all output except for Important messages"},
    {"verbose", 'v', 0, 0, "Output all messages"},
    {"structure", 's', 0, 0, "Print parsed structure then exit"},
    {"no-splash", 'n', 0, 0, "Disable splash screen/logo"},
    {0, 0, 0, 0}};

struct arguments {
  char *build_file;
  char *exec_script;
  bool check_file;
  char *mode;
  char *platform;
  bool disable_extensions;
  int log_level;
  bool print_structure;
  bool print_splash;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *args = state->input;
  switch (key) {
  case 'i':
    args->build_file = arg;
    break;
  case 'e':
    args->exec_script = arg;
    break;
  case 'c':
    args->check_file = true;
    break;
  case 'm':
    args->mode = arg;
    break;
  case 'p':
    args->platform = arg;
    break;
  case 'd':
    args->disable_extensions = true;
    break;
  case 'q':
    args->log_level = MB_LOGLVL_IMP;
    break;
  case 'v':
    args->log_level = MB_LOGLVL_LOW;
    break;
  case 's':
    args->print_structure = true;
    break;
  case 'n':
    args->print_splash = false;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, description};

/* Debugging function which prints the structure and
 * contents of a mcfg_file struct to stdout.
 */
void print_structure(struct mcfg_file *file) {
  printf("\n==========================\n\n");
  for (int i = 0; i < file->sector_count; i++) {
    mcfg_sector sector = file->sectors[i];
    printf("sector: %s\n", sector.name);
    printf("sections:\n");
    for (int j = 0; j < sector.section_count; j++) {
      mcfg_section section = sector.sections[j];
      printf("  section: %s\n", section.name);
      printf("  lines: %s\n", section.lines);
      printf("  fields:\n");
      for (int k = 0; k < section.field_count; k++) {
        mcfg_field field = section.fields[k];
        printf("    name: %s\n", field.name);
        printf("    value: %s\n", field.value);
      }
    }
  }
  printf("\n==========================\n");
}

const char logo[] =
    "\n\x1b[31m"
    "\x1b[1m█▀▄▀█ ▄▀█ █▀█ █ █▀▀ \x1b[0m\x1b[31m█▄▄ █ █ █ █   █▀▄\n"
    "\x1b[1m█ ▀ █ █▀█ █▀▄ █ ██▄ \x1b[0m\x1b[31m█▄█ █▄█ █ █▄▄ █▄▀\n"
    "\n\x1b[0m";

void print_logo() {
  printf(logo);
  printf("\x1b[1m\x1b[3m// version " VERSION_STRING " (" VERSION_TYPE
         ") //\x1b[0m\n");
  printf("\n");
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
  args.print_structure = false;
  args.print_splash = true;

  argp_parse(&argp, argc, argv, 0, 0, &args);

  mb_logging_level = args.log_level;
  if (args.print_splash)
    print_logo();

  struct mcfg_file *build_file = malloc_or_die(sizeof(mcfg_file));
  build_file->path = args.build_file;
  int result = parse_file(build_file);

  if ((result & MB_ERR_MASK_ERRNO) == MB_ERR_MASK_ERRNO)
    result = result ^ 0;

  if (result != 0) {
    mb_logf(MB_LOGLVL_IMP, "Parsing failed: Line %d\n", build_file->line);
    mb_logf(MB_LOGLVL_IMP, "Parsing failed: %s (0x%.8x) [%s]\n",
            errcode_msg(result), result, errclass_msg(result));
    if (mb_errtext != NULL)
      mb_logf(MB_LOGLVL_IMP, "--> %s\n", mb_errtext);
    mb_log(MB_LOGLVL_IMP, "Aborting build...\n");

    goto mb_exit;
  }

  if (args.check_file) {
    mb_log(MB_LOGLVL_IMP, "Build-File passed\n");
    goto mb_exit;
  }

  if (args.print_structure) {
    print_structure(build_file);
    goto mb_exit;
  }

  struct mb_exec_params exec_params;
  exec_params.exec_script = args.exec_script;
  exec_params.platform = args.platform;
  exec_params.mode = args.mode;
  exec_params.allow_extensions = !args.disable_extensions;

  result = mb_exec_build(build_file, exec_params);
  if (result != 0) {
    mb_logf(MB_LOGLVL_IMP, "Build failed: %s (0x%.8x) [%s]\n",
            errcode_msg(result), result, errclass_msg(result));
    if (mb_errtext != NULL)
      mb_logf(MB_LOGLVL_IMP, "--> %s\n", mb_errtext);
  } else {
    mb_log(MB_LOGLVL_STD, "Build succeeded!\n");
  }

mb_exit:
  free_mcfg_file(build_file);
  return result;
}
