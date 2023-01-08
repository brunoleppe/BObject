#ifndef B_ATOMIC_H
#define B_ATOMIC_H

#include <stdbool.h>

typedef struct _b_atomic_flag b_atomic_flag; 

bool b_atomic_flag_test_and_set(b_atomic_flag* flag);
void b_atomic_flag_clear(b_atomic_flag* flag);

#endif /*B_ATOMIC_H*/