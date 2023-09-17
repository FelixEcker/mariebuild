/* fsutils.h ; Filesystem Utility functions
 * Butter Utiltiy Library
 */

/* Licensed under the WTFPL version 2
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *  
 * Copyright (C) 2023, Marie Eckert
 * Copyright (C) 2010-2022, ThhE <thhe@gmx.de>
 * 
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *  
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 * 
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/* This library is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. 
 */

#ifndef BUTTER_FSUTILS_H
#define BUTTER_FSUTILS_H

/* Copies file at path from to file to.
 * Returns:
 *   0       - Successfully copied file
 *   nonzero - Failed to copy file, copy of errno is returned
 */
int copy_file(const char *to, const char *from);

/* Expands a Path like a POSIX shell.
 */
char *expand_path(char *path);

#endif
