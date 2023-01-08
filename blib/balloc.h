#ifndef B_ALLOC_H
#define B_ALLOC_H

#include <stddef.h>

#ifndef B_MALLOC
    #include <stdlib.h>
    #define B_MALLOC        malloc
#endif

#ifndef B_FREE
    #include <stdlib.h>
    #define B_FREE          free
#endif


static inline void* b_malloc(size_t size)
{
    return B_MALLOC(size);
}

static inline void b_free(void *ptr){
    return B_FREE(ptr);
}

#endif /*B_ALLOC_H*/