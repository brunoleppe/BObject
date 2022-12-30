#include "bobject.h"
#include <stdio.h>

static bType type_id = B_TYPE_OBJECT;
static bObjectClass class;

static void destructor(bObject* obj){
    b_type_free(obj,obj->type);
    DEBUG_PRINT("Eliminando el objeto\n");
}

static void bObject_class_initialize(bObjectClass* class){
    class->destructor = destructor;
    class->constructor = NULL;
}
static void bObject_instance_initialize(bObject* obj){
    obj->type = type_id;
    DEBUG_PRINT("inicializando bObject\n");
}

static void bOject_initialize(){
    static bool initialized = false;
    if(initialized)
        return;
    type_id = b_type_register(
        -1,
        sizeof(bObject),
        (void (*)(void*))bObject_instance_initialize,
        sizeof(bObjectClass),
        (void (*)(void*))bObject_class_initialize,
        &class);
    initialized = true;
    INFO_PRINT("Objecto inicializado\n");
    

}

bObject* bObject_new(bType type)
{
    bObject* obj = b_type_instantiate(type);
    if(obj == NULL)
        return NULL;
    obj->type = type;
    return obj;
}


void bObject_destructor(bObject* obj)
{
    bObjectClass* class = b_type_class_get(obj->type);
    class->destructor(obj);
}
void* bObject_constructor(void* params)
{
    // bObjectClass* class = bType_get_class(obj->type);
    // class->constructor(obj);
    return NULL;
}

bType bObject_get_type(){
    bOject_initialize();
    return type_id;
}