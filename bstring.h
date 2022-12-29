#ifndef BSTRING_H
#define BSTRING_H

#include "bobject.h"


typedef struct bString bString;
typedef struct bStringClass bStringClass;

struct bString{
    bObject parent_instance;
    char* string;
    int len;
};

struct bStringClass{
    bObjectClass parent_class;
    void (*print)(bString*);
};

bType b_string_get_type();
void b_string_print(bString* string);
bString* b_string_new(void);


#endif /*BSTRING_H*/