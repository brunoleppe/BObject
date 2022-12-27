#include "bObject.h"
#include "bstring.h"
#include <stdio.h>

int main(void){
    bOject_initialize();
    bString_initialize();
    
    bObject* obj = bObject_new();
    printf("Soy un objeto y mi tipo es %d\n",obj->type);

    bString* string = bString_new();
    bString_print(string);


    bObject_destructor(obj);
    bObject_destructor((bObject*)string);

    return 0;


}