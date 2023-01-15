/* debug.h: Print functions for debugging.
 * 
 * Copyright (C) 2023 Bruno Leppe <bruno.leppe.dev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef B_DEBUG
#define DEBUG_PRINT(...)        debug_print(DEBUG,__VA_ARGS__)     
#define INFO_PRINT(...)         debug_print(INFO,__VA_ARGS__)
#define ERROR_PINRT(...)         debug_print(ERROR,__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define INFO_PRINT(...)
#define ERROR_PINRT(...)
#endif

typedef enum DebugColors{
    INFO,
    DEBUG,
    ERROR,
}DebugColors;

/**
 * @brief Printf Wrapper with custom text colors.
 * 
 * @param color Text color 
 */
int debug_print(DebugColors color, char *fmt, ...);

#endif /*DEBUG_H*/