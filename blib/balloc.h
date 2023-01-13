/* balloc.h: malloc function wrapper for BObject.
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

#ifndef B_ALLOC_H
#define B_ALLOC_H

#include <stddef.h>

#ifndef B_MALLOC
    #include <stdlib.h>
    #define B_MALLOC        malloc
#endif

#ifndef B_FREE
    #include <stdlib.h>
    #define B_FREE          free
#endif

/**
 * @brief Wrapper for malloc type function.
 * 
 * @param size Size in bytes of memory block.
 * @return void* Pointer to the allocated memory, NULL if failure.
 */
static inline void* b_malloc(size_t size)
{
    return B_MALLOC(size);
}
/**
 * @brief Deallocates a block of memory previously allocated.
 * 
 * @param ptr Pointer to the memory block previously allocated.
 */
static inline void b_free(void *ptr){
    return B_FREE(ptr);
}

#endif /*B_ALLOC_H*/