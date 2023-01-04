#ifndef BTYPE_H
#define BTYPE_H

#include "debug.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int bType;
typedef size_t bSize;


#define B_TYPE_INTERFACE                (1)

#define B_TYPE_SHIFT_OBJECT             3
#define B_TYPE_MAKE_TPYE_ID(index)      ((index)<<B_TYPE_SHIFT_OBJECT)             
#define B_TYPE_MAKE_INDEX(type_id)      ((type_id)>>B_TYPE_SHIFT_OBJECT)


#define B_DECLARE_FINAL_TYPE(TypeName, type_name, ParentName)   \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##_get_type();\
struct TypeName##Class{\
    ParentName##Class parent_class;\
};

#define B_DECLARE_DERIVABLE_TYPE(TypeName, type_name , ParentName)  \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##_get_type();\


#define B_DEFINE_TYPE(TypeName, type_name, ParentType) B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, {})
#define B_DEFINE_TYPE_WITH_PRIVATE(TypeName, type_name, ParentType) B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, B_DEFINE_PRIVATE(TypeName,type_name))


#define B_DEFINE_PRIVATE(TN, t_n)                                   \
{\
    t_n##_private_offset = b_type_private_add(type_id,sizeof(TN##Private));\
}

#define B_DEFINE_TYPE_EXTENDED(TN, t_n, PT, _C_)     B_DEFINE_TYPE_INIT(TN,t_n,PT) {_C_;} B_DEFINE_TYPE_END()


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


#define B_DECLARE_INTERFACE(TypeName, type_name)                    \
typedef struct TypeName TypeName;\
typedef struct TypeName##Interface TypeName##Interface;\
bType type_name##_get_type();\
static inline TypeName##Interface* type_name##_get_iface(void* obj)\
{\
    return (TypeName##Interface*)b_type_interface_get(((BObject*)obj)->type,type_name##_get_type());\
} 

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
    INFO_PRINT(#TypeName" inicializado\n");                            \
}   

#define B_IMPLEMENT_INTERFACE(iface_type, init_fcn)\
{\
    b_type_interface_add(iface_type,type_id,(void (*)(void*))init_fcn);\
}

#define B_IMPLEMENT_INTERFACES(...)         \
{\
    b_type_add_interfaces(type_id, __VA_ARGS__, NULL);\
}

#define B_OVERWRITE_INTERFACE(iface_type, init_fcn)\
{\
    b_type_overwrite_interface(type_id,B_INTERFACE(iface_type, init_fcn));\
}

#define B_INTERFACE(_iface_type, _init_fcn)   &(IFaceParams){.iface_type = _iface_type, .init_fcn = (void (*)(void*))_init_fcn}

/********************************************************************************************************/
/*                              Structures                                                              */
/********************************************************************************************************/

typedef struct{
    bType iface_type;
    void (*init_fcn)(void*);
}IFaceParams;

/********************************************************************************************************/
/*                              Prototypes                                                              */
/********************************************************************************************************/


bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *klass
    );
int b_type_private_add(
    bType type,
    bSize private_size
    );
int b_type_interface_add(
    bType iface_type,
    bType type,
    void (*iface_initialize)(void*)
    );

void * b_type_class_get(bType type);
void * b_type_parent_class_get(bType type);

void * b_type_interface_get(bType instance_type, bType interface_type);

void * b_type_instantiate(bType type);
void   b_type_free(void* obj, bType type);

void b_type_add_interfaces(bType type, ... /* NULL*/);
void b_type_overwrite_interface(bType type, IFaceParams *params);
void b_type_class_initialize(bType type);
void b_type_interface_initialize(bType type);


bType b_type_object_initialize(
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *klass
    );

void b_type_clean();

#endif /*BTYPE_H*/
