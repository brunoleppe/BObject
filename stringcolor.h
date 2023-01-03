#ifndef STRINGCOLOR_H
#define STRINGCOLOR_H

#include "bstring.h"

#define STRING_TYPE_COLOR() string_color_get_type()
B_DECLARE_DERIVABLE_TYPE(StringColor,string_color,bString)

struct StringColor{
    bString parent_instance;
};

struct StringColorClass{
    bStringClass parent_class;
};

StringColor* string_color_new(void);

#endif /*STRINGCOLOR_H*/