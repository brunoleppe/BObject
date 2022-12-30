#include "btype.h"
#include <string.h>

typedef struct{
    bType type_id;
    bType parent_id;
    
    bSize instance_size;
    bSize class_size;
    
    bSize private_size;

    //const char* name;
    void* class;

    void (*constructor)(void*);

}bTypeNode;


static bTypeNode *types[20]; 
static bType type_id = 0;
static int type_count = 0;

bType b_type_register(
    bType parent_type,
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *class)
{
    type_count++;
    bTypeNode *q = malloc(sizeof(*q));
    
    q->instance_size = instance_size;
    q->class_size = class_size;

    q->type_id = type_id++;
    q->parent_id = parent_type;   

    q->class = class;

    if(q->parent_id >= 0){
        void *parent_class = types[parent_type]->class;
        bSize parent_class_size = types[parent_type]->class_size;
        //Inherit base class by copying all function pointers
        memcpy(q->class,parent_class,parent_class_size);
    }
    
    //Initialize the class
    class_initialize(q->class);

    q->constructor = instance_initialize;

    types[q->type_id] = q;

    return q->type_id;
}
int b_type_private_register(
    bType type,
    bSize private_size)
{
    bTypeNode *q = types[type];
    int offset = q->instance_size;
    q->instance_size += private_size;
    q->private_size = private_size;
    return offset;
}
void * b_type_class_get(bType type)
{
    bTypeNode *q = types[type];
    return q->class;
}
void * b_type_parent_class_get(bType type)
{
    bTypeNode *q = types[type];
    q = types[q->parent_id];
    return q->class;
}
static void b_type_initialize(bType type, void *instance)
{
    bTypeNode *q = types[type];
    if(q->parent_id>=0)
        b_type_initialize(q->parent_id,instance);
    q->constructor(instance);
}
void * b_type_instantiate(bType type)
{
    bTypeNode *q = types[type];
    void * instance = malloc(q->instance_size);
    if(instance == NULL)
        return NULL;
    b_type_initialize(type, instance);
    return instance;
}

void b_type_clean()
{
    int i;
    for(i=0;i<type_count;i++){
        free(types[i]);
    }
}


// bType bType_register(size_t instance_size, size_t class_size, void* class, void (*instance_initialize)(void), void (*class_initialize)(void))
// {
//     bTypeNode* q = malloc(sizeof(*q));
//     q->class = class;
//     q->instance_size = instance_size;
//     q->class_size = class_size;
//     q->name = "";
//     q->type_id = type_id;
//     type_id++;
//     types[q->type_id] = q;

//     return q->type_id;
// }
// int btype_register_private(bType type, size_t private_size)
// {
//     bTypeNode* q = types[type];
//     int offset = q->instance_size;
//     q->instance_size += private_size;
//     return offset;
// }
// void * bType_get_class(bType type)
// {
//     bTypeNode* q = types[type];
//     return q->class;
// }
// void * bType_instantiate(bType type)
// {
//     bTypeNode* q = types[type];
//     void * instance = malloc(q->instance_size);
//     return instance;
// }
