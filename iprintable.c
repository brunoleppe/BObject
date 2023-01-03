#include "iprintable.h"

B_DEFINE_INTERFACE(IPrintable, i_printable)

void i_printable_print(IPrintable* self)
{
    IPrintableInterface *iface = i_printable_get_iface(self);
    iface->print(self);
}