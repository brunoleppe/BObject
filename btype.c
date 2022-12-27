#include "bType.h"

//Array -> Arreglo de datos | dato 1| dato 2| dato 3| dato 4| | |
static bTypeQuery *types[20]; 
static bType type_id = 0;

typedef struct bTypeNode bTypeNode;

struct bTypeNode{
    bTypeNode *parent;
    bTypeQuery type_query;
}


bType bType_register(size_t instance_size, size_t class_size, void* class, void (*instance_initialize)(void), void (*class_initialize)(void))
{
    bTypeQuery* q = malloc(sizeof(*q));
    q->class = class;
    q->instance_size = instance_size;
    q->class_size = class_size;
    q->name = "";
    q->type_id = type_id;
    type_id++;
    types[q->type_id] = q;

    return q->type_id;
}
int btype_register_private(bType type, size_t private_size)
{
    bTypeQuery* q = types[type];
    int offset = q->instance_size;
    q->instance_size += private_size;
    return offset;
}
void * bType_get_class(bType type)
{
    bTypeQuery* q = types[type];
    return q->class;
}
void * bType_instantiate(bType type)
{
    bTypeQuery* q = types[type];
    void * instance = malloc(q->instance_size);
    return instance;
}