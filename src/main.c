#include <stdio.h>
#include <stdbool.h>

#include "argument_parse.h"
#include "logging.h"

#define MARIEBUILD_VERSION "0.4.0 (develop)"

struct args {
  char *target;
  char *buildfile;
  bool full;
  bool no_logo;
  bool keep_going; // they're hot on your heels!
  enum log_level verbosity;
};

int main(void) {

  return 0;
}
