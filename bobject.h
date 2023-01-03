#ifndef BOBJECT_H
#define BOBJECT_H

#include "btype.h"

#define B_TYPE_OBJECT()         b_object_get_type()


typedef struct bObject bObject;
struct bObject{
    bType type;
};

typedef struct bObjectClass bObjectClass;
struct bObjectClass{
    void (*destructor)(bObject*);
};

void b_object_delete(bObject* obj);

bType b_object_get_type();
bObject* b_object_new(bType type);

#endif /*BOBJECT_H*/