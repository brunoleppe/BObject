#ifndef BTYPE_H
#define BTYPE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef size_t bType;

typedef struct{
    size_t instance_size;
    size_t class_size;
    bType type_id;
    const char* name;
    void* class;
}bTypeQuery;

bType bType_register_with_name(char* name, size_t instance_size, size_t class_size, void* class);

bType bType_register(size_t instance_size, size_t class_size, void* class, void (*instance_initialize)(void), void (*class_initialize)(void));
int btype_register_private(bType type, size_t private_size);
void * bType_get_class(bType type);
void * bType_instantiate(bType type);

#endif /*BTYPE_H*/