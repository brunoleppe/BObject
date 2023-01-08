#include "blib/bobject.h"
#include "blib/blist.h"
#include <stdio.h>

static void delete_fcn(BObject* obj, void* data){
    b_object_delete(obj);
}

int main(void){

    BList *list = b_list_new();
    b_list_add(list,b_object_new(b_object_get_type()));
    b_list_add(list,b_object_new(b_object_get_type()));
    b_list_add(list,b_object_new(b_object_get_type()));
    b_list_add(list,b_object_new(b_object_get_type()));
    b_list_add(list,b_object_new(b_object_get_type()));

    printf("Cantidad de elementos: %d\n",b_list_count(list));

    b_list_delete(list,delete_fcn);



    b_type_clean();
    
    return 0;


}
