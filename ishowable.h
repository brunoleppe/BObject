#ifndef ISHOWABLE_H
#define ISHOWABLE_H

#include "bobject.h"

#define I_TYPE_SHOWABLE() i_showable_get_type()
B_DECLARE_INTERFACE(IShowable, i_showable)

struct IShowableInterface{
    void (*show)(IShowable *self);
};

void i_showable_show(IShowable* self);


#endif  /*ISHOWABLE_H*/