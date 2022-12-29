#include "bobject.h"
#include "bstring.h"
#include <stdio.h>

int main(void){

    bObject *obj = bObject_new(bObject_get_type());
    printf("Objeto con tipo %d\n",obj->type);

    bString *string = b_string_new();
    b_string_print(string);

    bString *string2 = b_string_new();
    b_string_print(string2);

    bObject_destructor((bObject*)string);
    bObject_destructor((bObject*)string2);

    return 0;


}