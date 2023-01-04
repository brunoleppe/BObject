#include "btest.h"
#include "iprintable.h"
#include "ishowable.h"
#include <stdio.h>

struct bTest{
    int a;
};

static void i_printable_interface_init(IPrintableInterface *iface);
static void i_showable_interface_init(IShowableInterface *iface);

B_DEFINE_TYPE_EXTENDED(bTest,b_test,B_TYPE_OBJECT(),
    // B_IMPLEMENT_INTERFACE(I_TYPE_PRINTABLE(),i_printable_interface_init)
    // B_IMPLEMENT_INTERFACE(I_TYPE_SHOWABLE(),i_showable_interface_init)
    B_IMPLEMENT_INTERFACES(
        B_INTERFACE(I_TYPE_PRINTABLE(),i_printable_interface_init),
        B_INTERFACE(I_TYPE_SHOWABLE(),i_showable_interface_init)
    )
)

static void i_printable_print_bTest(bTest * test)
{
    printf("Test Print de IPrintable\n");

}
static void i_printable_interface_init (IPrintableInterface *iface)
{
    iface->print = (void(*)(IPrintable*))i_printable_print_bTest;
}

static void i_showable_show_bTest(bTest * test){
    printf("Test Show de IShowable\n");
}

static void i_showable_interface_init(IShowableInterface *iface)
{
    iface->show = (void (*)(IShowable*))i_showable_show_bTest;
}


static void b_test_destructor(bTest* test)
{
    BObjectClass* class = b_type_parent_class_get(type_id);
    DEBUG_PRINT("Eliminando bTest\n");
    class->destructor((BObject*)test);
    
}

static void b_test_class_initialize(bTestClass* class){
    class->parent_class.destructor = (void(*)(BObject*))b_test_destructor;
}

static void b_test_instance_initialize(bTest* test)
{
    DEBUG_PRINT("Inicializando bTest\n");
    test->a = 10;
}

bTest* b_test_new(void)
{
    bTest* obj = (bTest*)b_object_new(b_test_get_type());
    return obj;
}