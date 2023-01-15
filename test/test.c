#include "bobject.h"
#include "blist.h"
#include "bqueue.h"
#include <stdio.h>

static void delete_fcn(BObject* obj, void* data){
    b_object_delete(obj);
}

int main(void){

    BQueue *q = b_queue_new(5);
    b_queue_enqueue(q, (void*)0);
    b_queue_enqueue(q, (void*)1);
    b_queue_enqueue(q, (void*)2);
    b_queue_enqueue(q, (void*)3);
    b_queue_enqueue(q, (void*)4);

    // b_queue_show(q);

    printf("\n%lu\n",(long long)b_queue_dequeue(q));
    printf("%lu\n",(long long)b_queue_dequeue(q));  

    // b_queue_show(q);

    b_queue_enqueue(q, (void*)6);

    printf("\n%lu\n",(long long)b_queue_dequeue(q));

    // b_queue_show(q);


    b_object_delete((BObject*)q);

    b_type_clean();
    
    return 0;


}
