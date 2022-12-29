#include "bstring.h"
#include <stdio.h>

typedef struct{
    int size;
}bStringPrivate;


static bType type_id;
static bStringClass class;
static int private_offset = 0;


static void destructor(bString* string)
{
    free(string->string);
    bObjectClass* class = b_type_parent_class_get(type_id);
    DEBUG_PRINT("Eliminando string\n");
    class->destructor((bObject*)string);
    
}
static void print(bString* string)
{
    printf("%s\n",string->string);
}


static void bString_class_initialize(bStringClass* class){
    class->parent_class.destructor = (void(*)(bObject*))destructor;
    class->print = print;
}

static void bString_instance_initialize(bString* string)
{
    bStringPrivate* priv = (bStringPrivate*)((char*)(string)+private_offset);  
    priv->size = 1024;
    string->string = malloc(priv->size + 1);
    string->parent_instance.type = type_id;
    DEBUG_PRINT("Inicializando bString\n");
}

void bString_initialize(){
    static bool initialized = false;
    if(initialized)
        return;
    type_id = b_type_register(
            B_TYPE_OBJECT,
            sizeof(bString),
            (void (*)(void*))bString_instance_initialize,
            sizeof(bStringClass),
            (void (*)(void*))bString_class_initialize,
            &class);

    private_offset = b_type_private_register(type_id,sizeof(bStringPrivate));
    initialized = true;
    INFO_PRINT("String inicializado\n");
}



bString* b_string_new(void)
{
    bString* obj = (bString*)bObject_new(b_string_get_type());
    return obj;
}

void b_string_print(bString* string)
{
    bStringClass* class = b_type_class_get(type_id);
    class->print(string);
}
bType b_string_get_type(){
    bString_initialize();
    return type_id;
}