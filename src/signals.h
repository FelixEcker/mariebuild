/* signals.h ; mariebuild build signal handling header
 *
 * Copyright (c) 2025, Marie Eckert
 * Licensend under the BSD 3-Clause License.
 */

#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

void mb_signal_generic_handler(int signal);

void mb_install_signal_handlers(void);

void mb_register_tmp_file(char *path);

void mb_unregister_tmp_file(char *path);

#endif
