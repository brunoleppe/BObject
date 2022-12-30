#include "bstringcolor.h"
#include "bobject.h"
#include "bstring.h"
#include <stdio.h>

static bType type_id;
static bStringColorClass class;
static int private_offset = 0;


static void destructor(bStringColor* string)
{
    bObjectClass* class = b_type_parent_class_get(type_id);
    DEBUG_PRINT("Eliminando string color\n");
    class->destructor((bObject*)string);
    
}


static void b_string_color_class_initialize(bStringColorClass* class){
    bObjectClass* parentClass = (bObjectClass*)class;
    parentClass->destructor = (void(*)(bObject*))destructor;
}

static void b_string_color_instance_initialize(bStringColor* string)
{
    DEBUG_PRINT("Inicializando bStringColor\n");
}

static void b_string_color_initialize(){
    static bool initialized = false;
    if(initialized)
        return;
    type_id = b_type_register(
            b_string_get_type(),
            sizeof(bStringColor),
            (void (*)(void*))b_string_color_instance_initialize,
            sizeof(bStringColorClass),
            (void (*)(void*))b_string_color_class_initialize,
            &class);

    initialized = true;
    INFO_PRINT("StringColor inicializado\n");
}



bStringColor* b_string_color_new(void)
{
    bStringColor* obj = (bStringColor*)bObject_new(b_string_color_get_type());
    return obj;
}

bType b_string_color_get_type(){
    b_string_color_initialize();
    return type_id;
}
