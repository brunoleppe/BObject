#include "bstring.h"



B_DEFINE_TYPE(BString, b_string, B_TYPE_OBJECT())

static void b_string_class_initialize(BStringClass *class)
{
	/*Implementation*/
}

static void b_string_instance_initialize(BString *self)
{
	/*Implementation*/
}

static void b_string_destructor(BString *self)
{
	/*Implementation*/
	BObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((BObject*)self);
	
}

BString* b_string_new(void){
	BString *obj = (BString*)b_object_new(b_string_get_type());
	return obj;
}