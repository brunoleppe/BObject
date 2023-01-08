#ifndef BOBJECT_H
#define BOBJECT_H

#include "btype.h"

#define B_TYPE_OBJECT()         b_object_get_type()


typedef struct BObject BObject;
struct BObject{
    bType type;
};

typedef struct BObjectClass BObjectClass;
struct BObjectClass{
    void (*destructor)(BObject*);
};

void b_object_delete(BObject* obj);

bType b_object_get_type();
BObject* b_object_new(bType type);

#endif /*BOBJECT_H*/