#include "bObject.h"
#include <stdio.h>

static bType type_id;

static bObjectClass class;

static void destructor(bObject* obj){
    free(obj);
    printf("Eliminando el objeto\n");
}

static void bObject_class_initialize(bObjectClass* class){
    class->destructor = destructor;
    class->constructor = NULL;
}
void bOject_initialize(){
    bObject_class_initialize(&class);
    
    type_id = bType_register(
            sizeof(bObject),
            sizeof(bObjectClass),
            &class,
            NULL,
            NULL);
}

void bObject_instance_initialize(bObject* obj){
    obj->type = type_id;
}

bObject* bObject_new(void)
{
    bObject* obj = bType_instantiate(type_id);
    bObject_instance_initialize(obj);
    return obj;
}


void bObject_destructor(bObject* obj)
{
    bObjectClass* class = bType_get_class(obj->type);
    class->destructor(obj);
}
void* bObject_constructor(void* params)
{
    // bObjectClass* class = bType_get_class(obj->type);
    // class->constructor(obj);
    return NULL;
}

bType bObject_get_type(){
    return type_id;
}