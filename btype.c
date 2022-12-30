#include "btype.h"
#include <string.h>

typedef struct{
    bType type_id;
    bType parent_id;
    
    bSize instance_size;
    bSize class_size;
    
    bSize private_size;
    int private_offset;

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
    //Count of registered classes for list of bTypeNode cleaning
    type_count++;

    //Alloc new bTypeNode
    bTypeNode *q = malloc(sizeof(*q));
    
    //Initialize private offset and size, will inherit parent's values later
    q->private_offset = 0;
    q->private_size = 0;

    //Assign instance size and class size 
    q->instance_size = instance_size;//used to alloc new object with total size (private + instance)
    q->class_size = class_size;//used to copy parent interface to child interface
    
    //Assign the new type_id and increment it
    q->type_id = type_id++;
    q->parent_id = parent_type;   
    //pointer to the object's class as void*
    q->class = class;

    //All classes should be derived from bObject, this should always be true except for bObject (base class)
    if(q->parent_id >= 0){
        bTypeNode *parent = types[q->parent_id];
        
        //obtain the parent class
        void *parent_class = parent->class;
        //obtain the parent class size
        bSize parent_class_size = parent->class_size;
        //Inherit base class by copying all function pointers to child class (this)
        memcpy(q->class,parent_class,parent_class_size);
        
        //update child's private offset and private size from parent.
        q->private_offset = parent->private_offset;
        q->private_size = parent->private_size;

    }
    
    //Initialize the class
    class_initialize(q->class);
    //Assign the object's constructor function
    q->constructor = instance_initialize;
    //Store the new bTypeNode
    types[q->type_id] = q;
    //return the bType corresponding to the newly registered class
    return q->type_id;
}
int b_type_private_register(
    bType type,
    bSize private_size)
{
    bTypeNode *q = types[type];
    q->instance_size += private_size;
    q->private_size += private_size;

    q->private_offset = q->private_size;

    return -q->private_offset;
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

    instance = ((char*)instance) + q->private_offset;

    b_type_initialize(type, instance);
    return instance;
}

void   b_type_free(void* obj, bType type)
{
    char *instance = (char*)obj;
    bTypeNode *q = types[type];
    char *pointer = (char*)(instance - q->private_offset);
    free(pointer);
}

void b_type_clean()
{
    int i;
    for(i=0;i<type_count;i++){
        free(types[i]);
    }
}