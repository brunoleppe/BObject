#include "bqueue.h"

typedef struct{
	int count;
	int front;
	int rear;
	int size;
	void **elements;
}BQueuePrivate;

B_DEFINE_TYPE_WITH_PRIVATE(BQueue, b_queue, B_TYPE_OBJECT())

static void b_queue_class_initialize(BQueueClass *class)
{
	/*Implementation*/
	((BObjectClass*)class)->destructor = b_queue_destructor;
}

static void b_queue_instance_initialize(BQueue *self)
{
	/*Implementation*/
	BQueuePrivate *priv = b_queue_get_private(self);
	priv->count = 0;
	priv->front = priv->rear = -1;
	priv->elements = NULL;
}

static void b_queue_destructor(BQueue *self)
{
	/*Implementation*/

	BQueuePrivate *priv = b_queue_get_private(self);
	b_free(priv->elements);

	BObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((BObject*)self);
	
}

BQueue* b_queue_new(int capacity){
	BQueue *obj = (BQueue*)b_object_new(b_queue_get_type());
	BQueuePrivate *priv = b_queue_get_private(obj);
	priv->size = capacity;
	priv->elements = b_malloc(sizeof(void*)*capacity);
	return obj;
}

void*		b_queue_enqueue		(BQueue *q, void *data)
{
	if(q == NULL)
		return NULL;
	BQueuePrivate *priv = b_queue_get_private(q);
	
	if((priv->front == 0 && priv->rear == priv->size-1) ||
		(priv->rear == (priv->front-1)%(priv->size-1)))
	{
		return NULL;
	}

	else if( priv->front == -1){
		priv->front = priv->rear = 0;
	}
	else if( priv->rear == priv->size - 1 && priv->front != 0){
		priv->rear = 0;
	}
	else{
		priv->rear++;
	}
	priv->elements[priv->rear] = data;
	priv->count++;
	return data;
}
void*		b_queue_dequeue		(BQueue *q)
{
	if(q == NULL)
		return NULL;
	BQueuePrivate *priv = b_queue_get_private(q);

	if(priv->front == -1){
		return NULL; //queue is empty
	}

	void *data = priv->elements[priv->front];
	
	if(priv->front == priv->rear){
		priv->front = priv->rear = -1;
	}
	else if(priv->front == priv->size-1)
		priv->front = 0;
	else
		priv->front++;
	priv->count--;

	return data;
}
int			b_queue_count		(BQueue *q, void *data)
{
	if(q == NULL)
		return 0;
	BQueuePrivate *priv = b_queue_get_private(q);

	return priv->count;
}
void		b_queue_delete		(BQueue *q, void (*delete_fcn)(void*))
{
	if(q == NULL)
		return;
	BQueuePrivate *priv = b_queue_get_private(q);
	if(delete_fcn != NULL){
		while(priv->count >0 )
			delete_fcn(b_queue_dequeue(q));
	}
	b_object_delete(q);
}

#include <stdio.h>
void 		b_queue_show		(BQueue *q)
{
	if(q == NULL)
		return;
	BQueuePrivate *priv = b_queue_get_private(q);
	if (priv->front == -1)
    {
        printf("\nQueue is Empty");
        return;
    }
    printf("\nElements in Circular Queue are: ");
    if (priv->rear >= priv->front)
    {
        for (int i = priv->front; i <= priv->rear; i++)
            printf("%d ",priv->elements[i]);
    }
    else
    {
        for (int i = priv->front; i < priv->size; i++)
            printf("%d ", priv->elements[i]);
 
        for (int i = 0; i <= priv->rear; i++)
            printf("%d ", priv->elements[i]);
    }
}