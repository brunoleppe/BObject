#include "bobject.h"
#include <stdio.h>

static bType type_id = 0;
static BObjectClass class;

static void destructor(BObject* obj){
    b_type_free(obj,obj->type);
    DEBUG_PRINT("Eliminando el objeto\n");
}

static void bObject_class_initialize(BObjectClass* class){
    class->destructor = destructor;
}
static void bObject_instance_initialize(BObject* obj){
    obj->type = type_id;
    DEBUG_PRINT("inicializando BObject\n");
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
    INFO_PRINT("Objecto inicializado\n");
    

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