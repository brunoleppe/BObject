/* bobject.h: base object class implementation.
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

#ifndef BOBJECT_H
#define BOBJECT_H

#include "btype.h"

/**
 * @brief Gets the type id associated to a BObject (0)
 * 
 */
#define B_TYPE_OBJECT()         b_object_get_type()


typedef struct BObject BObject;
struct BObject{
    bType type;
};

typedef struct BObjectClass BObjectClass;
struct BObjectClass{
    void (*destructor)(BObject*);
};

/**
 * @brief Deletes a BObject
 * 
 * @param obj Object to be deleted
 * 
 * This function class object destructors in reverese sequence for every inherited class.
 * The first destructor to be called is the last child's. 
 */
void b_object_delete(BObject* obj);


/**
 * @brief Gets the type ID associated to a BObject (0)
 * 
 * @return bType Type ID registered on the type system.
 */
bType b_object_get_type();


/**
 * @brief Instantiates a new object based on the type ID.
 * 
 * @param type Type ID of the object to be instantiated
 * @return BObject* Instantiated object.
 * 
 * This function calls object initializers in sequence for every inherited class.
 * the first initializer to be called is the first parent's.
 */
BObject* b_object_new(bType type);

#endif /*BOBJECT_H*/