#include "stringcolor.h"
#include "ishowable.h"
#include "iprintable.h"

static void i_showable_interface_init (IShowableInterface *iface);
static void i_printable_interface_init (IPrintableInterface* iface);

B_DEFINE_TYPE_EXTENDED(StringColor,string_color,B_TYPE_STRING(),
    B_IMPLEMENT_INTERFACES(
        B_INTERFACE(I_TYPE_SHOWABLE(),i_showable_interface_init)
    )
    B_OVERWRITE_INTERFACE(I_TYPE_PRINTABLE(),i_printable_interface_init)
)

static void i_showable_show_string_color(StringColor * sc)
{
    printf("Show de IShowable en StringColor\n");
}
static void i_printable_print_string_color(StringColor *sc)
{
    printf("Print de IPrintable en StringColor\n");
}


static void i_showable_interface_init (IShowableInterface *iface)
{
    iface->show = (void (*)(IShowable*))i_showable_show_string_color;
}
static void i_printable_interface_init (IPrintableInterface* iface)
{
    iface->print = (void (*)(IPrintable*))i_printable_print_string_color;
}

static void string_color_destructor(StringColor* string)
{
    BObjectClass* class = b_type_parent_class_get(type_id);
    DEBUG_PRINT("Eliminando StringColor\n");
    class->destructor((BObject*)string);
    
}



static void string_color_class_initialize(StringColorClass* class){
    BObjectClass *parent_class = (BObjectClass*)class;
    parent_class->destructor = (void (*)(BObject*))string_color_destructor;
}

static void string_color_instance_initialize(StringColor* string)
{
    DEBUG_PRINT("Inicializando StringColor\n");
}

StringColor* string_color_new(void)
{
    StringColor* obj = (StringColor*)b_object_new(string_color_get_type());
    return obj;
}