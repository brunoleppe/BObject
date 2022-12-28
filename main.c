#include "bobject.h"
// #include "bstring.h"
#include <stdio.h>

int main(void){

    bObject *obj = bObject_new(bObject_get_type());
    printf("Objeto con tipo %d\n",obj->type);

    return 0;


}