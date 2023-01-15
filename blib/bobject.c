/* bobject.c: base object class implementation.
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

#include "bobject.h"
#include <stdio.h>

/**
 * @brief 
 * 
 */
static bType type_id = 0;
static BObjectClass class;

static void destructor(BObject* obj){
    b_type_free(obj,obj->type);
    DEBUG_PRINT("Deleting Object\n");
}

static void bObject_class_initialize(BObjectClass* class){
    class->destructor = destructor;
}
static void bObject_instance_initialize(BObject* obj){
    obj->type = type_id;
    DEBUG_PRINT("Initializing BObject\n");
}

static void bOject_initialize(){
    static bool initialized = false;
    if(initialized)
        return;
    type_id = b_type_object_initialize(
        sizeof(BObject),
        (void (*)(void*))bObject_instance_initialize,
        sizeof(BObjectClass),
        (void (*)(void*))bObject_class_initialize,
        &class);

    b_type_class_initialize(type_id);

    initialized = true;
    INFO_PRINT("BObject Type Registered\n");
}

BObject* b_object_new(bType type)
{
    BObject* obj = b_type_instantiate(type);
    if(obj == NULL)
        return NULL;
    obj->type = type;
    return obj;
}


void b_object_delete(BObject* obj)
{
    BObjectClass* class = b_type_class_get(obj->type);
    class->destructor(obj);
}

bType b_object_get_type(){
    bOject_initialize();
    return type_id;
}