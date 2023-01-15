/* btype.c: Type System.
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

#include "btype.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "bobject.h"

/**
 * @brief Data structure containing interface parameters.
 * When implementing an interface, the class structure size is increased. Thus, 
 * an offset is provided to access the portion of memory where the interface is located.
 */
typedef struct IFaceEntry{
    bType class_type; /**< Interface's Type ID. */
    int offset; /**< Offset of the interface withing the class structure. */
    void (*init_fcn)(void*); /**< Pointer to the initialization function. */
    void *iface /**< Pointer to the interface structure */;
}IFaceEntry;

/**
 * @brief Structure containing interface data for a Type
 * Every Type in the Type System contains an IFaceData field.
 * 
 */
typedef struct {
    bool overridden; /**< Flag indicating that the entry_array of interfaces has been overriden in a derived class */
    int count; /**< count of implemented interfaces for a Type */
    int class_offset; /**< offset of the Type's class within the allocated class structure. */
    IFaceEntry *entry_array; /**< Array of implemented interfaces */
}IFaceData;

/**
 * @brief Structure that represents a Type in the Type System.
 * 
 */
typedef struct{
    bType type_id; /**< ID of the Type */
    bType parent_id; /**< Type ID of the parent */
    
    bSize instance_size; /**< Size in bytes of the instance structure (private + public + parent's private)*/
    bSize class_size; /**< Size in bytes of the class structure (class + implemented interfaces)*/
    
    bSize private_size; /**< Size of the private fields' structure */
    int private_offset /**< Offset of the private structure within the instance memory*/;

    //const char* name;
    void* class /**< Pointer to the class structure */;

    void (*constructor)(void*); /**< Instance initialization function */
    void (*class_init_fcn)(void*); /**< Class initialization function */

    IFaceData iface_data; /**< Data of implemented interfaces */
}bTypeNode;

/**
 * Array of TypeNodes that contains every registered Type.
 * 
 */
static bTypeNode *types[B_TYPE_MAX_TYPES]; 

/**
 * @brief Starting type_id for user defined Types.
 * Type ID 0 belongs to BObject.
 * Type ID 1 belongs to Interface types.
 * 
 */
static bType type_id = 2;

/**
 * @brief Maintains the count of registered Types.
 * 
 */
static int type_count = 0;

/**
 * @brief Gets the TypeNode associated to the specified Type ID.
 * 
 * @param type_id Type ID
 * @return bTypeNode* Associated TypeNode.
 */
static inline bTypeNode* b_type_get_node_from_type_id(bType type_id){
    //int index = B_TYPE_MAKE_INDEX(type_id);
    bTypeNode *q = types[type_id];
    return q;
}

/**
 * @brief Initializes the IFaceData field of a TypeNode.
 * 
 * @param data IFaceData field.
 */
static inline void _b_type_iface_data_initialize(IFaceData *data)
{
    data->class_offset = 0;
    data->count = 0;
    data->entry_array = NULL;
    data->overridden = false;
}

/**
 * @brief Copies the IFaceData of a parent Type to a derived Type.
 * 
 * @param dst Derived Type IFaceData field.
 * @param src Parent Type IFaceData field.
 */
static inline void _b_type_iface_data_copy(IFaceData *__restrict__ dst, IFaceData *__restrict__ src)
{
    dst->class_offset = src->class_offset;
    dst->count = src->count;
    dst->entry_array = dst->entry_array;
    //dst->overridden = dst->overridden; this is not copied as entry_array is not "new" and must be overriden
}

/**
 * @brief Copies the IFaceEntry array of a Parent Type to a Derived Type
 * 
 * The IFaceEntry structure contains the implemented interface parameters.
 * 
 * @param dst Derived Type IFaceData field. 
 * @param src Parent Type IfaceData field.
 */
static inline void _b_type_iface_data_inherit(IFaceData *__restrict__ dst, IFaceData *__restrict__ src)
{
    memcpy(
        dst->entry_array, 
        src->entry_array, 
        sizeof(IFaceEntry)*src->count
        );
    dst->overridden = true;  
}

bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *class)
{
    //Count of registered classes for list of bTypeNode cleaning
    type_count++;

    //Alloc new bTypeNode
    bTypeNode *q = b_malloc(sizeof(*q));
    if(!q){
        return -1;
    }

    //Initialize interface data
    _b_type_iface_data_initialize(&q->iface_data);

    //Initialize private offset and size, will inherit parent's values later
    q->private_offset = 0;
    q->private_size = 0;

    //Assign instance size and class size 
    q->instance_size = instance_size;//used to alloc new object with total size (private + instance)
    q->class_size = class_size;//used to copy parent interface to child interface
    
    //Assign the new type_id and increment it
    q->type_id = type_id++;
    q->parent_id = parent_type;   
    //pointer to the object's class as void*
    q->class = class;

    if(q->parent_id == 1){//Is Interface
        //q->data = i_face_list_create();
    }
    //All classes should be derived from BObject, this should always be true except for BObject (base class)
    else if(q->parent_id >= 0){
        bTypeNode *parent = types[q->parent_id];
        
        q->class_size += parent->iface_data.class_offset;
        
        //update child's private offset and private size from parent.
        q->private_offset = parent->private_offset;
        q->private_size = parent->private_size;
        //update instance size if private size != 0
        q->instance_size += q->private_size;

        //update iface_data
        _b_type_iface_data_copy(&q->iface_data,&parent->iface_data);
    }
    
    //Initialize the class
    q->class_init_fcn = class_initialize;
    //Assign the object's init function
    q->constructor = instance_initialize;
    //Store the new bTypeNode
    types[q->type_id] = q;

    //make type_id a proper type_id
    // q->type_id = B_TYPE_MAKE_TPYE_ID(q->type_id);

    //return the bType corresponding to the newly registered class
    return q->type_id;
}

bType b_type_object_initialize(
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *class
    )
{
       //Count of registered classes for list of bTypeNode cleaning
    type_count++;

    //Alloc new bTypeNode
    bTypeNode *q = b_malloc(sizeof(*q));
    if(!q){ //q == NULL
        return -1;
    }

    //Initialize interface data
    _b_type_iface_data_initialize(&q->iface_data);

    //Initialize private offset and size, will inherit parent's values later
    q->private_offset = 0;
    q->private_size = 0;

    //Assign instance size and class size 
    q->instance_size = instance_size;//used to alloc new object with total size (private + instance)
    q->class_size = class_size;//used to copy parent interface to child interface
    
    //Assign the new type_id and increment it
    q->type_id = 0;
    q->parent_id = -1;   
    //pointer to the object's class as void*
    q->class = class;

    //Initialize the class
    q->class_init_fcn = class_initialize;

    //Assign the object's constructor function
    q->constructor = instance_initialize;
    //Store the new bTypeNode
    types[q->type_id] = q;

    //make type_id a proper type_id
    // q->type_id = B_TYPE_MAKE_TPYE_ID(q->type_id);

    //return the bType corresponding to the newly registered class

    types[1] = NULL;

    return q->type_id;

}


int b_type_private_add(
    bType type,
    bSize private_size)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    q->instance_size += private_size;
    q->private_size += private_size;

    q->private_offset = q->private_size;

    return -q->private_offset;
}
void * b_type_class_get(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    return q->class;
}
void * b_type_parent_class_get(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    q = types[q->parent_id];
    return q->class;
}
static void b_type_initialize(bType type, void *instance)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    if(q->parent_id>=0)
        b_type_initialize(q->parent_id,instance);
    if(q->constructor)
        q->constructor(instance);
}
void * b_type_instantiate(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    void * instance = b_malloc(q->instance_size);
    if(instance == NULL)
        return NULL;

    instance = ((char*)instance) + q->private_offset;

    b_type_initialize(type, instance);
    return instance;
}

void   b_type_free(void* obj, bType type)
{
    char *instance = (char*)obj;
    bTypeNode *q = b_type_get_node_from_type_id(type);
    char *pointer = (char*)(instance - q->private_offset);
    free(pointer);
}

void b_type_clean()
{
    int i;
    for(i=0;i<=type_count;i++){
        if(types[i] == NULL)
            continue;
        
        free(((char*)types[i]->class)-types[i]->iface_data.class_offset);
        if(types[i]->iface_data.overridden)
            free(types[i]->iface_data.entry_array);
        
        free(types[i]);
    }
}

void * b_type_interface_get(bType instance_type, bType interface_type)
{
    bTypeNode *q = b_type_get_node_from_type_id(instance_type);
    if(!q->iface_data.entry_array){
        return NULL;
    }
    void *iface = NULL;
    int i;
    for(i=0;i<q->iface_data.count;i++){
        if(q->iface_data.entry_array[i].class_type == interface_type){
            iface = q->iface_data.entry_array[i].iface;
            break;
        }
    }
    return iface;
}

void b_type_add_interfaces(bType type, ...)
{
    bTypeNode *node = b_type_get_node_from_type_id(type);
    va_list params, list;
    va_start(params,type);
    va_copy(list, params);

    //initial interface count, inherited form parent class
    int count = node->iface_data.count;
    // int count = 0;

    IFaceParams *p = va_arg(list,IFaceParams*);
    while(p){
        count++;
        p = va_arg(list,IFaceParams*);
    }
    va_end(list);


    node->iface_data.entry_array = b_malloc(sizeof(IFaceEntry)*count);
    if(!node->iface_data.entry_array)
        return;
    if(node->parent_id >= 0 && node->iface_data.entry_array != NULL)
        _b_type_iface_data_inherit(&node->iface_data,&types[node->parent_id]->iface_data);
    

    int aux_count = node->iface_data.count;
    node->iface_data.count = count;
    IFaceEntry *entry_array = node->iface_data.entry_array;

    // int offset = 0;
    int offset = node->iface_data.class_offset;

    count = aux_count;
    // count = 0;    
    
    p = va_arg(params,IFaceParams*);
    while(p){
        bTypeNode *iface = b_type_get_node_from_type_id(p->iface_type);
        offset += iface->class_size;
        entry_array[count].class_type = iface->type_id;
        entry_array[count].init_fcn = p->init_fcn;
        entry_array[count].offset = offset;
        count++;

        node->class_size += iface->class_size;

        p = va_arg(params,IFaceParams*);
    }
    va_end(params);
    node->iface_data.class_offset = offset;
}

void b_type_class_initialize(bType type)
{
    bTypeNode *node = b_type_get_node_from_type_id(type);
    node->class = b_malloc(node->class_size);
    if(!node->class){
        return;
    }
    bTypeNode *parent = types[node->parent_id];
    if(node->parent_id >=0 && parent->iface_data.entry_array != NULL){
        int offset = 0;
        int i;
        for(i=node->iface_data.count-1; i>=parent->iface_data.count;i--){
            offset += types[node->iface_data.entry_array[i].class_type]->class_size;
        }
        //Inherit the parent class
        memcpy(
            ((char*)node->class) + offset,
            ((char*)parent->class)-parent->iface_data.class_offset,
            parent->class_size);
    }


    node->class = ((char*)node->class) + node->iface_data.class_offset;
    node->class_init_fcn(node->class);
    ((BObjectClass*)node->class)->type = type;

    int i;
    for(i=0;i<node->iface_data.count;i++){
        node->iface_data.entry_array[i].iface = ((char*)node->class) - node->iface_data.entry_array[i].offset;
        node->iface_data.entry_array[i].init_fcn(node->iface_data.entry_array[i].iface);
    }
}

void b_type_overwrite_interface(bType type, IFaceParams *params)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    if(!q->iface_data.overridden){
        q->iface_data.entry_array = b_malloc(sizeof(IFaceEntry)*q->iface_data.count);
        if(!q->iface_data.entry_array)
            return;

        if(q->parent_id >= 0 && q->iface_data.entry_array != NULL)
            _b_type_iface_data_inherit(&q->iface_data,&types[q->parent_id]->iface_data);
    }

    int i;
    for(i=0;i<q->iface_data.count;i++){
        if(q->iface_data.entry_array[i].class_type == params->iface_type){
            q->iface_data.entry_array[i].init_fcn = params->init_fcn;
            return;
        }
    }
}