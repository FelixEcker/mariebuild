// build.h ; mariebuild build functionality header
//
// Copyright (c) 2024, Marie Eckert
// Licensend under the BSD 3-Clause License.
//------------------------------------------------------------------------------

#ifndef BUILD_H
#define BUILD_H

#include "mcfg.h"

#include "logging.h"
#include "strlist.h"

typedef struct args {
  char *buildfile;
  char *target;
  bool force;
  bool no_splash;
  bool keep_going; // they're hot on your heels!
  log_level_t verbosity;
} args_t;

typedef struct config {
  char *default_target;
  char *target;
  strlist_t public_targets;
  bool always_force;
  bool ignore_failures;
} config_t;

typedef struct build_state {
  mcfg_section_t *target;
  mcfg_section_t *current_target;
  mcfg_section_t *current_rule;
} build_state_t;

int mb_start(args_t args);

int mb_begin_build(mcfg_file_t *file, config_t cfg);

#endif // #ifndef BUILD_H
