/* ANSI Terminal Control Sequence Macros.
 * Author: Marie Eckert <marie@ha.si>
 * Licensed under the WTFPL (http://www.wtfpl.net/txt/copying/).
 */

#ifndef ANSI_H
#define ANSI_H

#define ESC "\x1b"

/* Stylings */

#define ANSI_RESET ESC "[0m"
#define ANSI_BOLD ESC "[1m"
#define ANSI_DIM ESC "[2m"
#define ANSI_ITALIC ESC "[3m"
#define ANSI_UNDERLINE ESC "[4m"
#define ANSI_SBLINK ESC "[5m"
#define ANSI_RBLINK ESC "[6m"
#define ANSI_INVERT ESC "[7m"
#define ANSI_HIDE ESC "[8m"
#define ANSI_STRIKE ESC "[9m"
#define ANSI_DEFAULT ESC "[10m"

/* 3/4 bit Foreground Colors */

#define ANSI_FG_BLACK ESC "[30m"
#define ANSI_FG_RED ESC "[31m"
#define ANSI_FG_GREEN ESC "[32m"
#define ANSI_FG_YELLOW ESC "[33m"
#define ANSI_FG_BLUE ESC "[34m"
#define ANSI_FG_MAGENTA ESC "[35m"
#define ANSI_FG_CYAN ESC "[36m"
#define ANSI_FG_WHITE ESC "[37m"
#define ANSI_FG_BBLACK ESC "[90m"
#define ANSI_FG_BRED ESC "[91m"
#define ANSI_FG_BGREEN ESC "[92m"
#define ANSI_FG_BYELLOW ESC "[93m"
#define ANSI_FG_BBLUE ESC "[94m"
#define ANSI_FG_BMAGENTA ESC "[95m"
#define ANSI_FG_BCYAN ESC "[96m"
#define ANSI_FG_BWHITE ESC "[97m"

/* 3/4 bit Background Colors */

#define ANSI_BG_BLACK ESC "[40m"
#define ANSI_BG_RED ESC "[41m"
#define ANSI_BG_GREEN ESC "[42m"
#define ANSI_BG_YELLOW ESC "[43m"
#define ANSI_BG_BLUE ESC "[44m"
#define ANSI_BG_MAGENTA ESC "[45m"
#define ANSI_BG_CYAN ESC "[46m"
#define ANSI_BG_WHITE ESC "[47m"
#define ANSI_BG_BBLACK ESC "[100m"
#define ANSI_BG_BRED ESC "[101m"
#define ANSI_BG_BGREEN ESC "[102m"
#define ANSI_BG_BYELLOW ESC "[103m"
#define ANSI_BG_BBLUE ESC "[104m"
#define ANSI_BG_BMAGENTA ESC "[105m"
#define ANSI_BG_BCYAN ESC "[106m"
#define ANSI_BG_BWHITE ESC "[107m"

#endif /* #ifndef ANSI_H */
