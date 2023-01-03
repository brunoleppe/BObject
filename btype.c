#include "btype.h"
#include <string.h>
#include <stdio.h>

typedef struct IFaceEntry{
    bType class_type;
    void *iface;
}IFaceEntry;

typedef struct IFaceNode{
    IFaceEntry entry;
    struct IFaceNode *next;
}IFaceNode;

typedef struct IFaceList{
    IFaceNode *head;
    IFaceNode *current;
    int count;
}IFaceList;

static IFaceList* i_face_list_create( void );
static void i_face_list_add(IFaceList*, bType type, void *iface);
static void * i_face_list_get_iface(IFaceList*, bType type);


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

    void *data;

}bTypeNode;


static bTypeNode *types[20];
static bType type_id = 2;
static int type_count = 0;

static inline bTypeNode* b_type_get_node_from_type_id(bType type_id){
    //int index = B_TYPE_MAKE_INDEX(type_id);
    bTypeNode *q = types[type_id];
    return q;
}



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

    if(q->parent_id == 1){//Is Interface
        q->data = i_face_list_create();
    }
    //All classes should be derived from bObject, this should always be true except for bObject (base class)
    else if(q->parent_id >= 0){
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
        //update instance size if private size != 0
        q->instance_size += q->private_size;
    }
    
    //Initialize the class
    if(class_initialize)
        class_initialize(q->class);
    //Assign the object's constructor function
    q->constructor = instance_initialize;
    //Store the new bTypeNode
    types[q->type_id] = q;

    //make type_id a proper type_id
    // q->type_id = B_TYPE_MAKE_TPYE_ID(q->type_id);

    //return the bType corresponding to the newly registered class
    return q->type_id;
}

bType b_type_object_initialize(
    bSize instance_size,
    void (*instance_initialize)(void*),
    bSize class_size,
    void (*class_initialize)(void*),
    void *class
    )
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
    q->type_id = 0;
    q->parent_id = -1;   
    //pointer to the object's class as void*
    q->class = class;

    //Initialize the class
    if(class_initialize)
        class_initialize(q->class);
    //Assign the object's constructor function
    q->constructor = instance_initialize;
    //Store the new bTypeNode
    types[q->type_id] = q;

    //make type_id a proper type_id
    // q->type_id = B_TYPE_MAKE_TPYE_ID(q->type_id);

    //return the bType corresponding to the newly registered class

    types[1] = NULL;

    return q->type_id;

}


int b_type_private_add(
    bType type,
    bSize private_size)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    q->instance_size += private_size;
    q->private_size += private_size;

    q->private_offset = q->private_size;

    return -q->private_offset;
}
void * b_type_class_get(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    return q->class;
}
void * b_type_parent_class_get(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    q = types[q->parent_id];
    return q->class;
}
static void b_type_initialize(bType type, void *instance)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    if(q->parent_id>=0)
        b_type_initialize(q->parent_id,instance);
    if(q->constructor)
        q->constructor(instance);
}
void * b_type_instantiate(bType type)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
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
    bTypeNode *q = b_type_get_node_from_type_id(type);
    char *pointer = (char*)(instance - q->private_offset);
    free(pointer);
}

void b_type_clean()
{
    int i;
    for(i=0;i<=type_count;i++){
        if(types[i] == NULL)
            continue;
        
        if(types[i]->parent_id == 1){
            IFaceList *list = (IFaceList*)types[i]->data;
            IFaceNode *node = list->head;
            IFaceNode *aux;
            while(node){
                aux = node;
                free(node->entry.iface);
                node = node->next;
                free(aux);
            }
            free(list);

        }

        
        free(types[i]);
    }
}

static IFaceList* i_face_list_create( void )
{
    IFaceList *list = malloc(sizeof(*list));
    list->count = 0;
    list->head = NULL;
    list->current = NULL;
    return list;
}
static void i_face_list_add(IFaceList* list, bType type, void *iface)
{
    IFaceNode *node = malloc(sizeof(*node));
    node->entry.class_type = type;
    node->entry.iface = iface;
    node->next = NULL;

    if(list->head == NULL){
        list->head = node;
    }
    else{
        list->current->next = node;
    }
    list->current = node;
    list->count++;
}
static void * i_face_list_get_iface(IFaceList* list, bType type)
{
    IFaceNode *node = list->head;
    while(node){
        if(node->entry.class_type == type)
            return node->entry.iface;
        node = node->next;
    }
    return NULL;
}

int b_type_interface_add(
    bType iface_type,
    bType type,
    void (*iface_initialize)(void*)
    )
{
    bTypeNode *q = types[iface_type];
    IFaceList *list = q->data;
    void *iface = malloc(q->class_size);
    iface_initialize(iface);
    i_face_list_add(list,type,iface);
    return 0;
}

void * b_type_interface_get(bType instance_type, bType interface_type)
{
    bTypeNode *q = types[instance_type];
    IFaceList *list = types[interface_type]->data;
    void *iface;
    do{
        iface = i_face_list_get_iface(list, q->type_id);
        if(q->parent_id < 0){
            break;
        }
        q = types[q->parent_id];
        
    }while(iface == NULL);
    return iface;
}