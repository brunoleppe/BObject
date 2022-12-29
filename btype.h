#ifndef BTYPE_H
#define BTYPE_H

#include "debug.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int bType;
typedef size_t bSize;


#define B_TYPE_OBJECT       (0)

bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *class
    );
int b_type_private_register(
    bType type,
    bSize private_size
    );
void * b_type_class_get(bType type);
void * b_type_parent_class_get(bType type);

void * b_type_instantiate(bType type);

#endif /*BTYPE_H*/