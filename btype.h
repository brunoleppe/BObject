#ifndef BTYPE_H
#define BTYPE_H

#include "debug.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int bType;
typedef size_t bSize;


#define B_TYPE_OBJECT       (0)

#define B_DECLARE_FINAL_TYPE(TypeName, type_name, ParentName)   \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##get_type();\
struct TypeName##Class{\
    ParentName##Class parent_class;\
};

#define B_DECLARE_DERIVABLE_TYPE(TypeName, type_name , ParentName)  \
typedef struct TypeName TypeName;\
typedef struct TypeName##Class TypeName##Class;\
bType type_name##get_type();\

#define B_DEFINE_TYPE(TypeName, type_name, ParentType) B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, {})
#define B_DEFINE_TYPE_WITH_PRIVATE(TypeName, type_name, ParentType) B_DEFINE_TYPE_EXTENDED(TypeName, type_name, ParentType, B_DEFINE_PRIVATE())


#define B_DEFINE_PRIVATE(TN, t_n)                                   \
    t_n##_private_offset = b_type_private_register(type_id,sizeof(TN##Private));

#define B_DEFINE_TYPE_EXTENDED(TN, t_n, PT, _C_)     B_DEFINE_TYPE_INIT(TN,t_n,PT) {_C_;} B_DEFINE_TYPE_END()


#define B_DEFINE_TYPE_INIT(TypeName,type_name,ParentType)           \
static bType type_id;                                               \
static TypeName##Class class;                                       \
static int type_name##_private_offset                               \
                                                                    \
static inline void* type_name##_get_private(TypeName* self)         \
{                                                                   \
    return ((char*)self) + type_name##_private_offset;              \
}                                                                   \
static void type_name##_destructor(TypeName* self);                 \
static void type_name##_class_initialize(TypeName##Class* class);   \
static void type_name##_instance_initialize(TypeName* self);        \
static void type_name##_initialize(){                               \
    static bool initialized = false;                                \
    if(initialized)                                                 \
        return;                                                     \
    initialized = true;                                             \
    type_id = b_type_register(                                      \
            ParentType,                                             \
            sizeof(TypeName),                                       \
            (void (*)(void*))type_name##_instance_initialize,       \
            sizeof(bStringClass),                                   \
            (void (*)(void*))type_name##_class_initialize,          \
            &class);                                                \
    {
#define B_DEFINE_TYPE_END()                                         \
    }                                                               \
    initialized = true;                                             \
    INFO_PRINT("String inicializado\n");                            \
}   


bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *klass
    );
int b_type_private_register(
    bType type,
    bSize private_size
    );
void * b_type_class_get(bType type);
void * b_type_parent_class_get(bType type);

void * b_type_instantiate(bType type);
void   b_type_free(void* obj, bType type);



void b_type_clean();

#endif /*BTYPE_H*/
