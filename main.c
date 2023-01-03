#include "bobject.h"
#include "bstring.h"
#include <stdio.h>
#include "iprintable.h"
#include "btest.h"
#include "ishowable.h"
#include "stringcolor.h"

int main(void){
    bObject *obj = b_object_new(b_object_get_type());
    


    bString *string = b_string_new();
    b_string_set(string, "Hola Mundo");

    
    bTest *test = b_test_new();

    i_printable_print((IPrintable*)string);
    i_printable_print((IPrintable*)test);
    i_showable_show((IShowable*)test);
    
    b_object_delete((bObject*)string);
    b_object_delete(obj);
    b_object_delete((bObject*)test);
    
    StringColor *sc = string_color_new();
    b_string_set((bString*)sc,"Cosas");
    i_printable_print((IPrintable*)sc);
    i_showable_show((IShowable*)sc);
    b_string_print((bString*)sc);

    b_object_delete((bObject*)sc);
    


    b_type_clean();
    

    return 0;


}
