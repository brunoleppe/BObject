/* btype.h: Type System.
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

#ifndef BTYPE_H
#define BTYPE_H

#include "debug.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "balloc.h"

typedef int bType;
typedef size_t bSize;

/**
 * @brief Type ID for Interfaces, non instatiable classes.
 * 
 */
#define B_TYPE_INTERFACE                (1)

/**
 * @brief Maximum number of clases that can be registered
 * 
 */
#define B_TYPE_MAX_TYPES                (255)

/** Defined but not Used */
#define B_TYPE_SHIFT_OBJECT             3
#define B_TYPE_MAKE_TPYE_ID(index)      ((index)<<B_TYPE_SHIFT_OBJECT)             
#define B_TYPE_MAKE_INDEX(type_id)      ((type_id)>>B_TYPE_SHIFT_OBJECT)


/**
 * @brief Macro for declaring a class that will not be subclased.
 * These classes don't define virtual methods. Thus, the class definition is hidden within this macro.
 * The class structure inherits the parent's class. All new class should be derived from BObject.
 * 
 * Derived class can only have one parent class - Single Inheritance
 * 
 * Example for a new class called MyClass with BObject as parent class:
 * 
 * myclass.h:
 * 
 * #define MY_TYPE_CLASS()      my_class_get_type()
 * B_DECLARE_FINAL_TYPE(MyClass, my_class, BObject)
 * 
 * MyClass* my_clas_new(void);
 * 
 * Example for a new class called DerivedClass derived from MyClass:
 * 
 * derivedclass.h:
 * 
 * #define DERIVED_TYPE_CLASS()   derived_class_get_type()
 * B_DECLARE_FINAL_TYPE(DerivedClass, derived_class, MyClass)
 * 
 * DerivedClass* derived_class_new(void);
 */
#define B_DECLARE_FINAL_TYPE(TypeName, type_name, ParentName)   \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##_get_type();\
struct TypeName##Class{\
    ParentName##Class parent_class;\
};


/**
 * @brief Macro for declaring a class that will be subclased.
 * The class structure is now befined by the user, virtual functions can now be implemented.
 * The class structure must inherit the parent's class.
 * 
 * Derived class can only have one parent class - Single Inheritance
 * 
 * Example for a new class called MyObject with BObject as parent class:
 * 
 * myobject.h:
 * 
 * #define MY_TYPE_Object()      my_object_get_type()
 * B_DECLARE_FINAL_TYPE(MyObject, my_object, BObject)
 * 
 * struct MyObjectClass{
 *      BObjectClass *parent_class;
 *      void (*virtual_method_1)(MyObject*);
 *      void (*virtual_method_2)(MyObject*);
 * };
 * 
 * MyObject* my_object_new(void);
 * 
 */
#define B_DECLARE_DERIVABLE_TYPE(TypeName, type_name , ParentName)  \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##_get_type();\

/**
 * @brief Macro for defining a new class.
 * Defines the class with no extra implementations (private fields, interfaces).
 * This macro should be used in source files (.c) 
 */
#define B_DEFINE_TYPE(TypeName, type_name, ParentType)                      B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, {})

/**
 * @brief Macro for defining a new class with private fields.
 * Defines the class with an extra private structure tha user defines.
 * This macro should be used in source files (.c) 
 * 
 * Example for a new class called MyObject with BObject as parent Class, user must define th
 * private struct before this macro as follows:
 * 
 * myobject.c:
 * 
 * typedef struct{
 *      type private_field_1;
 *      type_private:field_2;
 * }MyObjectPrivate;
 * 
 * B_DEFINE_CLASS_WITH_PRIVATE(MyObject,my_object,BObject)
 * 
 * 
 */
#define B_DEFINE_TYPE_WITH_PRIVATE(TypeName, type_name, ParentType)         B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, B_DEFINE_PRIVATE(TypeName,type_name))

/**
 * @brief Macro for adding a private struct to the newly created type.
 * This macro should be used as follows:
 * 
 * B_DEFINE_TYPE_EXTENDED(MyObject, my_object, BObject, 
 *      B_DEFINE_PRIVATE(MyObject, my_object);
 * )
 * 
 */
#define B_DEFINE_PRIVATE(TN, t_n)                                   \
{\
    t_n##_private_offset = b_type_private_add(type_id,sizeof(TN##Private));\
}

/**
 * @brief Macro for defining a class with extra code.
 * This macro is used to define types with private fields or interfaces.
 * 
 */
#define B_DEFINE_TYPE_EXTENDED(TN, t_n, PT, _C_)                            B_DEFINE_TYPE_INIT(TN,t_n,PT) {_C_;} B_DEFINE_TYPE_END()


#define B_DEFINE_TYPE_INIT(TypeName,type_name,ParentType)           \
static bType type_id;                                               \
static TypeName##Class class;                                       \
static int type_name##_private_offset;                              \
                                                                    \
static inline void* type_name##_get_private(TypeName* self)         \
{                                                                   \
    return ((char*)self) + type_name##_private_offset;              \
}                                                                   \
static void type_name##_destructor(TypeName* self);                 \
static void type_name##_class_initialize(TypeName##Class* class);   \
static void type_name##_instance_initialize(TypeName* self);        \
static void type_name##_initialize();                               \
bType type_name##_get_type()                                        \
{                                                                   \
    type_name##_initialize();                                       \
    return type_id;                                                 \
}                                                                   \
static void type_name##_initialize(){                               \
    static bool initialized = false;                                \
    if(initialized)                                                 \
        return;                                                     \
    initialized = true;                                             \
    type_id = b_type_register(                                      \
            ParentType,                                             \
            sizeof(TypeName),                                       \
            (void (*)(void*))type_name##_instance_initialize,       \
            sizeof(TypeName##Class),                                   \
            (void (*)(void*))type_name##_class_initialize,          \
            &class);                                                \
    {
#define B_DEFINE_TYPE_END()                                         \
    }                                                               \
    b_type_class_initialize(type_id);\
    initialized = true;                                             \
}   

/**
 * @brief Macro for declaring a non instantiable class, aka Interface
 * A struct TypeName is created empty. The class struct must be defined
 * by the user. 
 * 
 * Interfaces aren't derivable at this time; but could be, as an interface is
 * just another class in the type system.
 * 
 * A class can implement any number of interfaces.
 * 
 * Example for a new interface IDrawable
 * 
 * idrawable.h:
 * 
 * #include <bobject.h>
 * 
 * #define I_TYPE_DRAWABLE()    i_drawable_get_type();
 * B_DECLARE_INTERFACE(IDrawable, i_drawable)
 * 
 * struct IDrawableInterface{
 *      void (*draw)(IDrawable*);
 * };
 * 
 * void i_drawable_draw(IDrawable *self);
 * 
 */
#define B_DECLARE_INTERFACE(TypeName, type_name)                    \
typedef struct TypeName TypeName;\
typedef struct TypeName##Interface TypeName##Interface;\
bType type_name##_get_type();\
static inline TypeName##Interface* type_name##_get_iface(void* obj)\
{\
    return (TypeName##Interface*)b_type_interface_get(((BObject*)obj)->type,type_name##_get_type());\
} 

/**
 * @brief Macro for defining interfaces.
 * This macro should be used in source files (.c)
 * 
 */
#define B_DEFINE_INTERFACE(TypeName, type_name)\
static bType type_id;                                               \
static void type_name##_initialize();                               \
bType type_name##_get_type()                                        \
{                                                                   \
    type_name##_initialize();                                       \
    return type_id;                                                 \
}                                                                   \
static void type_name##_initialize(){                               \
    static bool initialized = false;                                \
    if(initialized)                                                 \
        return;                                                     \
    initialized = true;                                             \
    type_id = b_type_register(                                      \
            B_TYPE_INTERFACE,                                             \
            0,                                       \
            NULL,       \
            sizeof(TypeName##Interface),                                   \
            NULL,          \
            NULL);                                                \
    initialized = true;                                             \
    INFO_PRINT("Initializing Interface "#TypeName"\n");                            \
}   

/**
 * @brief Macro for implementing interfaces in new classes.
 * 
 * This macro is used as follows:
 * 
 * MyObject Class, implements IDrawable, IPrintable
 * 
 * void i_drawable_initialize(IDrawable *iface);
 * void i_printable_initialize(IPrintable *iface);
 * 
 * B_DEFINE_TYPE_EXTENDED(MyObject, my_object, BObject,
 *      B_IMPLEMET_INTERFACES(
 *          B_INTERFACE(IDrawable, i_drawable_initialize),
 *          B_INTERFACE(IPrintable, i_printable_initialize)
 *      )
 * ) 
 * 
 */
#define B_IMPLEMENT_INTERFACES(...)         \
{\
    b_type_add_interfaces(type_id, __VA_ARGS__, NULL);\
}

/**
 * @brief Macro for overwriting parent interfaces in derived classes.
 * 
 * This macro is used for overriding one or all interface methods of one or all interfaces
 * from a parent class.
 * 
 * Is used as follows:
 * DerivedObject Class, overrides IPrintable, IDrawable
 * 
 * void i_drawable_initialize(IDrawable *iface);
 * void i_printable_initialize(IPrintable *iface);
 * 
 * B_DEFINE_TYPE_EXTENDED(MyObject, my_object, BObject,
 *      B_OVERRIDE_INTERFACE(B_INTERFACE(IDrawable, i_drawable_initialize))
 *      B_OVERRIDE_INTERFACE(B_INTERFACE(IPrintable, i_printable_initialize))
 * ) 
 * 
 */
#define B_OVERRIDE_INTERFACE(b_interface)\
{\
    b_type_override_interface(type_id, b_interface);\
}

#define B_INTERFACE(_iface_type, _init_fcn)   \
&(IFaceParams){.iface_type = _iface_type, .init_fcn = (void (*)(void*))_init_fcn}

/**********************************************************************************************************************/
/*                              Structures                                                                            */
/**********************************************************************************************************************/

/**
 * @brief Structure used for passing interface data as parameters.
 * This structure is used in b_type_add_interfaces and b_type_overwrite_interface functions.
 * 
 */
typedef struct{
    bType iface_type; /**< Type of the registered interface */
    void (*init_fcn)(void*); /**< Initialization function */
}IFaceParams;

/***********************************************************************************************************************/
/*                              Prototypes                                                                             */
/***********************************************************************************************************************/

/**
 * @brief Function used to register new types in the type system.
 * This function is used to register the size of objects, size of classes initialization functions and the parent type.
 * 
 * When creating a new Type in the Type System, the Type's class is created once and is shared among all instances of 
 * the Type. The Type's class is then initialized using the class_initialize function.
 * 
 * When creating derived classes, the parent class is copied to the derived class to maintain the virtual methods.
 * Virtual methods are overriden when the class_initialize function is called, if new implementations are provided.
 * 
 * The instance size is increased if the parent's type contains private data.
 * 
 * @param parent_type Type ID of the parent class
 * @param instance_size Size of the object's data structure.  
 * @param instance_initialize Pointer to the object's initialization function.
 * @param class_size Size of the object's class structure
 * @param class_initialize Pointer to the object's class initialization function.
 * @param klass Deprecated, passed as NULL;
 * @return bType The Type ID associated to the newly registered class. Returns -1 in case of error.
 */
bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *klass
    );

/**
 * @brief Function used to register a structure of private fields within a newly registered type.
 * 
 * This function increases the instance size of the type and provides an offset to the memory block containing the 
 * private fields.
 * 
 * The private structure is placed below the public fields portion of memory: if an object is located at address 
 * 0x1FC0D0B0 and the private structure is 16 bytes, private structure is located at 0x1FC0D0A0.
 * 
 * @param type Type ID of the newly registered class.
 * @param private_size Size of the private structure.
 * @return int Offset of the private structure.
 */
int b_type_private_add(
    bType type,
    bSize private_size
    );

/**
 * @brief Function used to get the class of the specified Type ID.
 * 
 * @param type Type ID.
 * @return void* pointer to the class structure of the specified Type ID.
 */
void * b_type_class_get(bType type);

/**
 * @brief Function used to get the class of the parent's Type of the specified Type ID.
 * 
 * @param type Type ID
 * @return void* Pointer to the class structure of the parent's Type.
 */
void * b_type_parent_class_get(bType type);

/**
 * @brief Function used to get the interface structure of the specified Type ID providing the interface ID.
 * 
 * @param instance_type Type ID of the object instance.
 * @param interface_type Type ID of the desired interface.
 * @return void* Pointer to the interface structure.
 */
void * b_type_interface_get(bType instance_type, bType interface_type);

/**
 * @brief Function used to instantiate a new object of the specified type ID.
 * This function allocates a memory block of the instance size provided by the specified Type ID.
 * 
 * @param type Type ID.
 * @return void* Pointer to the newly created object.
 */
void * b_type_instantiate(bType type);

/**
 * @brief Function used to free the memory allocated for an instance of the specified Type ID.
 * 
 * @param obj Pointer to the instance.
 * @param type Type ID of the instance.
 */
void   b_type_free(void* obj, bType type);

/**
 * @brief Function used to register interfaces implemented by the specified Type ID.
 * 
 * Interfaces are an extension to the class structure instance of the Type ID.
 * The size of the class increments with every registered interface's size
 * 
 * @param type Type ID.
 * @param ... Variable arguments of type IFaceParams, NULL terminated
 * 
 */
void b_type_add_interfaces(bType type, ... /* NULL*/);

/**
 * @brief Function used to provide a new implementation for an interface of a parent class in a derived class.
 * 
 * @param type Type ID of the derived class.
 * @param params IFaceParams structure containing the Type ID of the interface and the new interface initialization
 * function.
 */
void b_type_override_interface(bType type, IFaceParams *params);

/**
 * @brief Function used to initialize the instance of the class structure of the specified Type ID.
 * 
 * This function allocates a memory block with the total size of the class (class structure plus interfaces), 
 * then calls the class_initialize function and interface_initialize function for every registered interface.  
 * 
 * @param type Type ID.
 */
void b_type_class_initialize(bType type);

/**
 * @brief Function used to initialize the BObject Type.
 * Must not be used.
 * 
 */
bType b_type_object_initialize(
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *klass
    );

/**
 * @brief Function used to free the memory allocated by the type system.
 * 
 */
void b_type_clean();

#endif /*BTYPE_H*/
