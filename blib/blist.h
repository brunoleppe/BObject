/* blist.h: linked list implementation.
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

#ifndef BLIST_H
#define BLIST_H

#include "bobject.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Gets the type id associated to a BList Object
 * 
 */
#define B_TYPE_LIST()		b_list_get_type()
B_DECLARE_FINAL_TYPE(BList, b_list, BObject)
/**
 * @brief BList data type, public fields.
 * 
 */
struct BList{
	BObject parent_instance;
	/*public fields*/
};
/**
 * @brief BList Constructor
 * 
 * @return BList* Pointer to BList object
 */
BList* b_list_new( void );


/**
 * @brief Adds a new element to the list.
 * 
 * @param list This BList.
 * @param data Data to be added.
 */
void	b_list_add				(BList* list, void *data);


/**
 * @brief Gets the current element count.
 * 
 * @param list This BList.
 * @return int Element count.
 */
int 	b_list_count			(BList* list);


/**
 * @brief Inserts a new element to the list at the desired position.
 * 
 * @param list This BList.
 * @param data Data to be inserted.
 * @param index Position of the list.
 */
void	b_list_insert			(BList* list, void *data, int index);


/**
 * @brief Gets the data at the node specified by the desired position.
 * 
 * @param list This BList
 * @param index Position of the data.
 * @return void* Pointer to the existing data, NULL if index out of bounds.
 */
void* 	b_list_index 			(BList* list, int index);


/**
 * @brief Calls a function for every element of the list.
 * 
 * @param list This BList.
 * @param func A function pointer with two void* parameters. 
 * @param user_data Data to be passed as extra argument to the function.
 */
void	b_list_foreach			(BList* list, void (*func)(void* element, void* user_data), void* user_data);


/**
 * @brief Clears all the elements of the list and calls a clean up function if defined.
 * The clean up function should free the memory allocated for a single element if needed.
 * @param list This BList.
 * @param clear_func Clean up function. If NULL, elements won't be cleaned (if working with integers, const char*, etc).
 */
void	b_list_clear			(BList* list, void (*clear_func)(void*, void* user_data));


/**
 * @brief Free all resources used by the list and class a clean up function to free resources used by the elements. 
 * 
 * @param list This BList.
 * @param clear_func Clean up function. If NULL, elements won't be cleaned (if working with integers, const char*, etc).
 */
void    b_list_delete			(BList* list, void (*clear_func)(void*, void* user_data));


/**
 * @brief Sets the current element pointer to the first element of the list.
 * 
 * @param list This BList.
 * 
 * This function is used to initialize the list iterator to the first position.
 */
void	b_list_set_at_origin	(BList* list);


/**
 * @brief Sets the current element pointer to the last element of the list;
 * 
 * @param list This BList
 * 
 * This function is used to initialize the list iterator to the last position.
 */
void	b_list_set_at_end		(BList* list);


/**
 * @brief Returns the current element and moves the current element pointer to the next element.
 * 
 * @param list This BList
 * @return void* Data at the current element pointer before moving it to the next element.
 */
void*	b_list_next				(BList* list);


/**
 * @brief Returns the current element and moves the current element pointer to the previous element.
 * 
 * @param list This BList.
 * @return void* Data at the current element pointer before moving it to the previous element. 
 */
void* 	b_list_prev				(BList* list);


#ifdef __cplusplus
}
#endif


#endif /*BLIST_H*/