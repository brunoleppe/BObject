#include "bobject.h"
#include "bstring.h"
#include "bstringcolor.h"
#include <stdio.h>
#include "iprintable.h"
#include "btest.h"
#include "ishowable.h"

int main(void){
    bObject *obj = b_object_new(b_object_get_type());
    


    bString *string = b_string_new();
    b_string_set(string, "Hola Mundo");
    // b_string_print(string);
    // b_string_print_secret(string);

    
    bTest *test = b_test_new();

    i_printable_print((IPrintable*)string);
    i_printable_print((IPrintable*)test);
    i_showable_show((IShowable*)test);

    bStringColor *sc = b_string_color_new();
    
    // b_string_set((bString*)sc, "Cosas");
    // b_string_print((bString*)sc);
    // b_string_print_secret((bString*)sc);
    // b_string_color_print_secret(sc);
    i_printable_print((IPrintable*)sc);
    
    b_object_delete((bObject*)string);
    b_object_delete(obj);
    b_object_delete((bObject*)test);
    b_object_delete((bObject*)sc);
    

    
    b_type_clean();
    

    return 0;


}
