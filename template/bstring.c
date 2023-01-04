#include "bstring.h"


typedef struct{
	/*private fields*/
}BStringPrivate;

B_DEFINE_TYPE_WITH_PRIVATE(BString, b_string, B_TYPE_OBJECT())

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
	bObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((bObject*)self);
	
}

BString* b_string_new(void){
	BString *obj = (BString*)b_object_new(b_string_get_type());
	return obj;
}