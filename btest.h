#ifndef BTEST_H
#define BTEST_H

#include "bobject.h"

#define B_TYPE_TEST() b_test_get_type()
B_DECLARE_FINAL_TYPE(bTest,bTest,bObject)


bTest* b_test_new(void);

#endif /*BTEST_H*/