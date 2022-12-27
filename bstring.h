#ifndef BSTRING_H
#define BSTRING_H

#include "bObject.h"


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


void bString_print(bString* string);
bString* bString_new(void);
void bString_initialize();

#endif /*BSTRING_H*/