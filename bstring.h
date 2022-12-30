#ifndef BSTRING_H
#define BSTRING_H

#include "bobject.h"

#define B_TYPE_STRING() b_string_get_type()
B_DECLARE_DERIVABLE_TYPE(bString,b_string,bObject)

struct bString{
    bObject parent_instance;
    char *string;
    int len;
};

struct bStringClass{
    bObjectClass parent_class;
    void (*print)(bString*);
};

void b_string_print(bString* string);
bString* b_string_new(void);
void b_string_set(bString* str, char* s);
void b_string_print_secret(bString* str);
#endif /*BSTRING_H*/