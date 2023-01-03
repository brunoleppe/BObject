#ifndef IPRINTABLE_H
#define IPRINTABLE_H

#include "bobject.h"

#define I_TYPE_PRINTABLE() i_printable_get_type()
B_DECLARE_INTERFACE(IPrintable, i_printable)

struct IPrintableInterface{
    void (*print)(IPrintable *self);
};

void i_printable_print(IPrintable* self);


#endif  /*IPRINTABLE_H*/

