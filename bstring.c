#include "bstring.h"
#include <stdio.h>
#include <string.h>

typedef struct{
    int size;
    char *secret;
}bStringPrivate;

B_DEFINE_TYPE_WITH_PRIVATE(bString, b_string, B_TYPE_OBJECT())


// static bType type_id;
// static bStringClass class;
// static int private_offset = 0;


static void b_string_destructor(bString* string)
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


static void b_string_class_initialize(bStringClass* class){
    class->parent_class.destructor = (void(*)(bObject*))b_string_destructor;
    class->print = print;
}

static void b_string_instance_initialize(bString* string)
{
    DEBUG_PRINT("Inicializando bString\n");
    bStringPrivate* priv = b_string_get_private(string);
    priv->size = 64;
    string->string = malloc(priv->size);
    priv->secret = "String Sectreto base";
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

void b_string_set(bString* str, char* s)
{
    strcpy(str->string,s);
}
void b_string_print_secret(bString* str)
{
    bStringPrivate* priv = b_string_get_private(str);      
    printf("%s\n",priv->secret);
}