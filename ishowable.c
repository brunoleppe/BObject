#include "ishowable.h"

B_DEFINE_INTERFACE(IShowable, i_showable)

void i_showable_show(IShowable* self)
{
    IShowableInterface *iface = i_showable_get_iface(self);
    iface->show(self);
}