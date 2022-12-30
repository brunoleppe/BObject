#ifndef BSTRINGCOLOR_H
#define BSTRINGCOLOR_H

#include "bstring.h"

typedef struct bStringColor bStringColor;
typedef struct bStringColorClass bStringColorClass;

struct bStringColor{
    bString parent_instance;
};

struct bStringColorClass{
    bStringClass parent_class;
};

bType b_string_color_get_type();
bStringColor* b_string_color_new(void);



#endif
