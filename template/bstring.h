#ifndef BSTRING_H
#define BSTRING_H

#include "bobject.h"

#ifdef __cplusplus
extern "C"{
#endif


#define B_TYPE_STRING()		b_string_get_type()
B_DECLARE_FINAL_TYPE(BString, b_string, BObject)

struct BString{
	BObject parent_instance;
	/*public fields*/
};

BString* b_string_new( void );
/*Class Methods*/


#ifdef __cplusplus
}
#endif


#endif /*BSTRING_H*/