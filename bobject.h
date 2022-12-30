#ifndef BOBJECT_H
#define BOBJECT_H

#include "btype.h"

typedef struct bObject bObject;
struct bObject{
    bType type;
};

typedef struct bObjectClass bObjectClass;
struct bObjectClass{
    void (*destructor)(bObject*);
};

void bObject_destructor(bObject* obj);
void* bObject_constructor(void* params);

bType bObject_get_type();
bObject* bObject_new(bType type);

#endif /*BOBJECT_H*/