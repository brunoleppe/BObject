/* bqueue.h: Queue implementation.
 * 
 * Copyright (C) 2023 Bruno Leppe <bruno.leppe.dev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

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
/**
 * @brief Adds a new element to the queue.
 * 
 * @param q This queue.
 * @param data Data to be added to the queue.
 * @return void* If en element was succesfully added, returns the element. Returns NULL
 * if the queue is full.
 */
void*		b_queue_enqueue		(BQueue *q, void *data);

/**
 * @brief Gets the first element of the queue and removes it.
 * 
 * @param q This queue.
 * @return void* Returns the first element of the queue. Returns NULL if the queue is empty.
 */
void*		b_queue_dequeue		(BQueue *q);

/**
 * @brief Gets the number of elements enqueued.
 * 
 * @param q This List.
 * @return int Number of elements enqueued.
 */
int			b_queue_count		(BQueue *q);

/**
 * @brief Frees memory allocated for the queue and calls a delete function for every element 
 * inside the queue if defined.
 * 
 * @param q This queue.
 * @param delete_fcn Function to free the memory allocated for a single element of the queue
 */
void		b_queue_delete		(BQueue *q, void (*delete_fcn)(void*));

/**Debug Function*/
void 		b_queue_show		(BQueue *q);

#ifdef __cplusplus
}
#endif


#endif /*BQUEUE_H*/