#include "bobject.h"
#include "bstring.h"
#include "bstringcolor.h"
#include <stdio.h>

int main(void){

    bObject *obj = bObject_new(bObject_get_type());
    printf("Objeto con tipo %d\n",obj->type);

    bString *string = b_string_new();
    

    bString *string2 = b_string_new();
    
    bStringColor *sc = b_string_color_new();
    if(sc != NULL){
        b_string_set((bString*)sc, "Cosas");
        b_string_print((bString*)sc);
        bObject_destructor((bObject*)sc);
    }

    b_string_set(string, "Hola Mundo");
    b_string_set(string2, "Chao Mundo");
    
    b_string_print(string);
    b_string_print(string2);

    

    bObject_destructor((bObject*)string);
    bObject_destructor((bObject*)string2);
    bObject_destructor(obj);


    
    b_type_clean();
    

    return 0;


}
