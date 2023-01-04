#include "btype.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>



typedef struct IFaceEntry{
    bType class_type;
    int offset;
    void (*init_fcn)(void*);
    void *iface;
}IFaceEntry;

typedef struct {
    int count;
    int class_offset;
    IFaceEntry *entry_array;
}IFaceData;

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
    void (*class_init_fcn)(void*);

    void *data;
    IFaceData iface_data;
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
    
    //Initialize interface data
    q->iface_data.class_offset = 0;
    q->iface_data.count = 0;
    q->iface_data.entry_array = NULL;

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
        //q->data = i_face_list_create();
    }
    //All classes should be derived from BObject, this should always be true except for BObject (base class)
    else if(q->parent_id >= 0){
        bTypeNode *parent = types[q->parent_id];
        
        q->class_size += parent->iface_data.class_offset;

        // //obtain the parent class
        // void *parent_class = parent->class;
        // //obtain the parent class size
        // bSize parent_class_size = parent->class_size;
        // //Inherit base class by copying all function pointers to child class (this)
        // memcpy(q->class,parent_class,parent_class_size);
        
        //update child's private offset and private size from parent.
        q->private_offset = parent->private_offset;
        q->private_size = parent->private_size;
        //update instance size if private size != 0
        q->instance_size += q->private_size;

        //update iface_data
        q->iface_data = parent->iface_data;
    }
    
    //Initialize the class
    q->class_init_fcn = class_initialize;
    // if(class_initialize)
    //     class_initialize(q->class);
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
    
    //Initialize interface data
    q->iface_data.class_offset = 0;
    q->iface_data.count = 0;
    q->iface_data.entry_array = NULL;

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
    q->class_init_fcn = class_initialize;
    // if(class_initialize)
    //     class_initialize(q->class);
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
        
        free(((char*)types[i]->class)-types[i]->iface_data.class_offset);
        if(types[i]->iface_data.entry_array)
            free(types[i]->iface_data.entry_array);

        // if(types[i]->parent_id == 1){
        //     IFaceList *list = (IFaceList*)types[i]->data;
        //     IFaceNode *node = list->head;
        //     IFaceNode *aux;
        //     while(node){
        //         aux = node;
        //         free(node->entry.iface);
        //         node = node->next;
        //         free(aux);
        //     }
        //     free(list);

        // }

        
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
    void *iface = NULL;
    int i;
    for(i=0;i<q->iface_data.count;i++){
        if(q->iface_data.entry_array[i].class_type == interface_type){
            iface = q->iface_data.entry_array[i].iface;
            break;
        }
    }
    // IFaceList *list = types[interface_type]->data;
    // void *iface;
    // do{
    //     iface = i_face_list_get_iface(list, q->type_id);
    //     if(q->parent_id < 0){
    //         break;
    //     }
    //     q = types[q->parent_id];
        
    // }while(iface == NULL);
    return iface;
}

void b_type_add_interfaces(bType type, ...)
{
    bTypeNode *node = types[type];
    va_list params, list;
    va_start(params,type);
    va_copy(list, params);

    //initial interface count, inherited form parent class
    int count = node->iface_data.count;
    // int count = 0;

    IFaceParams *p = va_arg(list,IFaceParams*);
    while(p){
        count++;
        p = va_arg(list,IFaceParams*);
    }
    va_end(list);


    node->iface_data.entry_array = malloc(sizeof(IFaceEntry)*count);
    if(node->parent_id >= 0 && node->iface_data.entry_array != NULL){
        memcpy(
            node->iface_data.entry_array, 
            types[node->parent_id]->iface_data.entry_array, 
            sizeof(IFaceEntry)*types[node->parent_id]->iface_data.count
            );
    }

    int aux_count = node->iface_data.count;
    node->iface_data.count = count;
    IFaceEntry *entry_array = node->iface_data.entry_array;

    // int offset = 0;
    int offset = node->iface_data.class_offset;

    count = aux_count;
    // count = 0;    
    
    p = va_arg(params,IFaceParams*);
    while(p){
        bTypeNode *iface = types[p->iface_type];
        offset += iface->class_size;
        entry_array[count].class_type = iface->type_id;
        entry_array[count].init_fcn = p->init_fcn;
        entry_array[count].offset = offset;
        count++;

        node->class_size += iface->class_size;

        p = va_arg(params,IFaceParams*);
    }
    va_end(params);
    node->iface_data.class_offset = offset;
}

void b_type_class_initialize(bType type)
{
    bTypeNode *node = types[type];
    node->class = malloc(node->class_size);
    bTypeNode *parent = types[node->parent_id];
    if(node->parent_id >=0 && parent->iface_data.entry_array != NULL){
        int offset = 0;
        int i;
        for(i=node->iface_data.count-1; i>=parent->iface_data.count;i--){
            offset += types[node->iface_data.entry_array[i].class_type]->class_size;
        }
        //Inherit the parent class
        memcpy(
            ((char*)node->class) + offset,
            ((char*)parent->class)-parent->iface_data.class_offset,
            parent->class_size);
    }


    node->class = ((char*)node->class) + node->iface_data.class_offset;
    node->class_init_fcn(node->class);


    int i;
    for(i=0;i<node->iface_data.count;i++){
        node->iface_data.entry_array[i].iface = ((char*)node->class) - node->iface_data.entry_array[i].offset;
        node->iface_data.entry_array[i].init_fcn(node->iface_data.entry_array[i].iface);
    }
}

void b_type_overwrite_interface(bType type, IFaceParams *params)
{
    bTypeNode *q = b_type_get_node_from_type_id(type);
    int i;
    for(i=0;i<q->iface_data.count;i++){
        if(q->iface_data.entry_array[i].class_type == params->iface_type){
            q->iface_data.entry_array[i].init_fcn = params->init_fcn;
            return;
        }
    }
}

// void b_type_interface_initialize(bType type)
// {
//     bTypeNode *node = b_type_get_node_from_type_id(type);
//     int i;
//     for(i=0;i<node->iface_data.count;i++){
//         node->iface_data.entry_array[i].iface = ((char*)node->class) - node->iface_data.entry_array[i].offset;
//         node->iface_data.entry_array[i].init_fcn(node->iface_data.entry_array[i].iface);
//     }
// }