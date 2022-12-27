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
    free(string);
    printf("Eliminando string\n");
}
static void print(bString* string)
{
    printf("%s\n",string->string);
}


static void bString_class_initialize(bStringClass* class){
    class->parent_class.destructor = (void(*)(bObject*))destructor;
    class->print = print;
}
void bString_initialize(){
    bString_class_initialize(&class);
    
    type_id = bType_register(
            sizeof(bString),
            sizeof(bStringClass),
            &class,
            NULL,
            NULL);

    private_offset = btype_register_private(type_id,sizeof(bStringPrivate));

}

void bString_instance_initialize(bString* string)
{
    bStringPrivate* priv = (bStringPrivate*)((char*)(string)+private_offset);  
    priv->size = 1024;
    string->string = "Te amo mucho Salito\n"; 
    string->parent_instance.type = type_id;
}

bString* bString_new(void)
{
    bString* obj = bType_instantiate(type_id);
    bString_instance_initialize(obj);
    return obj;
}

void bString_print(bString* string)
{
    bStringClass* class = bType_get_class(type_id);
    class->print(string);
}