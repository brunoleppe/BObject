/* blist.c: linked list implementation.
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

#include "blist.h"

/**
 * @brief Doubly Linked List node structure.
 * Contains a pointer to the user data and two pointers to the next and previous node.
 * 
 */
typedef struct BListNode{
	void *data;
	struct BListNode *prev;
	struct BListNode *next;
}BListNode;

/**
 * @brief Private BList fields.
 * 
 */
typedef struct{
	/*private fields*/
	int count; /**< Mantains the count of elements inside the list*/
	BListNode *head; /**< Pointer to the first node of the list*/
	BListNode *tail; /**< Pointer to the last node of the list*/
	BListNode *current; /**< Pointer to the current element of the list, used to traverse the list with
						the b_list_next and b_list_prev methods.*/
}BListPrivate;

/**
 * @brief Frees all the nodes of the list.
 * Doesn't free the data. 
 * 
 * @param list This list.
 */
static void _b_list_free(BList* list);


B_DEFINE_TYPE_WITH_PRIVATE(BList, b_list, B_TYPE_OBJECT())

static void b_list_class_initialize(BListClass *class)
{
	/*Implementation*/
	((BObjectClass*)class)->destructor = (void (*)(BObject*))b_list_destructor;
}

static void b_list_instance_initialize(BList *self)
{
	/*Implementation*/
	BListPrivate *priv = b_list_get_private(self);
	priv->count = 0;
	priv->head = NULL;
	priv->tail = NULL;
	priv->current = NULL;
}

static void b_list_destructor(BList *self)
{
	/*Implementation*/

	_b_list_free(self);

	BObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((BObject*)self);
		
}

BList* b_list_new(void){
	BList *obj = (BList*)b_object_new(b_list_get_type());
	return obj;
}

void _b_list_free(BList *self)
{
	BListPrivate *priv = b_list_get_private(self);
	BListNode *node = priv->head;
	BListNode *aux = node;

	while(node){
		aux = node->next;
		free(node);
		node = aux;
	}

	priv->count = 0;
	priv->head = priv->tail = NULL;
}
void	b_list_add		(BList* list, void *data)
{
	if(list == NULL)
		return;
	BListPrivate *priv = b_list_get_private(list);
	BListNode *node = b_malloc(sizeof(*node));
	if(!node)
		return;
	node->data = data;
	node->prev = NULL;
	node->next = NULL;

	if(priv->head == NULL){
		priv->head = node;
		priv->current = priv->head;
	}
	else{
		priv->tail->next = node;
		node->prev = priv->tail;
	}

	priv->tail = node;
	priv->count++;
}
int 	b_list_count	(BList* list)
{
	if(list == NULL)
		return 0;
	BListPrivate *priv = b_list_get_private(list);
	return priv->count;
}
void	b_list_insert	(BList* list, void *data, int index)
{

}
void* 	b_list_index 	(BList* list, int index)
{
	if(list == NULL)
		return NULL;
	BListPrivate *priv = b_list_get_private(list);
	BListNode *node = priv->head;
	while( (index-- > 0) && node)
		node = node->next;
	return node? node->data : NULL;
}
void	b_list_foreach	(BList* list, void (*func)(void *element, void *user_data), void *user_data)
{
	if(list == NULL)
		return;
	if(func == NULL)
		return;

	BListPrivate *priv = b_list_get_private(list);
	BListNode *node = priv->head;
	while(node){
		func(node->data, user_data);
		node = node->next;
	}
}	
void	b_list_clear	(BList* list, void (*clear_func)(void*, void *user_data))
{
	b_list_foreach(list,clear_func,NULL);
	_b_list_free(list);
}
void    b_list_delete	(BList* list, void (*clear_func)(void*, void *user_data))
{
	b_list_foreach(list,clear_func,NULL);
	b_object_delete((BObject*)list);
}
void	b_list_set_at_origin	(BList* list)
{
	if(list == NULL)
		return;
	BListPrivate *priv = b_list_get_private(list);
	priv->current = priv->head;
}
void	b_list_set_at_end		(BList* list)
{
	if(list == NULL)
		return;
	BListPrivate *priv = b_list_get_private(list);
	priv->current = priv->tail;
}
void*	b_list_next				(BList* list)
{
	if(list == NULL)
		return NULL;
	BListPrivate *priv = b_list_get_private(list);
	
	if(priv->current == NULL)
		priv->current = priv->head;

	void* data = priv->current->data;
	priv->current = priv->current->next;
	return data;

}
void* 	b_list_prev				(BList* list)
{
	if(list == NULL)
		return NULL;
	BListPrivate *priv = b_list_get_private(list);
	
	if(priv->current == NULL)
		priv->current = priv->tail;

	void* data = priv->current->data;
	priv->current = priv->current->prev;
	return data;
}