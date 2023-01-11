#ifndef BQUEUE_H
#define BQUEUE_H

#include "bobject.h"

#ifdef __cplusplus
extern "C"{
#endif


#define B_TYPE_QUEUE()		b_queue_get_type()
B_DECLARE_FINAL_TYPE(BQueue, b_queue, BObject)

struct BQueue{
	BObject parent_instance;
	/*public fields*/
};

BQueue* b_queue_new( int capacity );
/*Class Methods*/
void*		b_queue_enqueue		(BQueue *q, void *data);
void*		b_queue_dequeue		(BQueue *q);
int			b_queue_count		(BQueue *q, void *data);
void		b_queue_delete		(BQueue *q, void (*delete_fcn)(void*));
void 		b_queue_show		(BQueue *q);

#ifdef __cplusplus
}
#endif


#endif /*BQUEUE_H*/