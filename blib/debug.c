/* debug.c: Print functions for debugging.
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

#include "debug.h"
#include <stdarg.h>

char *colors[] = {
    "\033[0;34m",
    "\033[0;32m",
    "\033[0;31m"
};

int debug_print(DebugColors color, char *fmt, ...)
{
    printf("%s",colors[color]);

    va_list list;
    va_start(list,fmt);
    int res = vprintf(fmt,list);
    printf("\033[0m"); 

    return res;
}