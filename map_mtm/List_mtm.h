//
//	List_mtm.h
// Created by:
//	Shay Ben-Ishay 308052463
//	Liav Eliyahu 311502660
//
#ifndef HW3_LIST_MTM_H
#define HW3_LIST_MTM_H

#include <stdbool.h>
/**
* Generic List Container
*
* The following functions are available:
*
*   listCreate               - Creates a new empty list
*   listDestroy              - Deletes an existing list and frees all resources
*   listCopy                 - Copies an existing list
*   listGetSize              - Returns the size of a given list
*   listGetData              - Returns the data of a given node
*	listSetData              - Sets data for a given node and free it's previous data.
*   listGetKey               - Returns the key of a given node
*   listRemoveCurrent        - Removes the element
*   listGetNext              - Advances the list's node to the next element and returns it
*   listInsertElement		 - Insert an element to his correct place in the list. (the list is sorted by Keys)
*   keyGetNode				 - Gets key and list and returns the node that contains this key
*
*Internal static function:
*	listGetNextKey			 - return MapKeyElement pointer to the key of the next node
*/

/** Type for defining the list */
typedef struct List *Node;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
	LIST_SUCCESS,
	LIST_NULL_ARGUMENT,
	LIST_OUT_OF_MEMORY,
	LIST_INVALID_CURRENT,
} ListResult;

/**
* Allocates a new List.
*
* Creates a new empty list.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new List in case of success.
*/
Node listCreate();

/**
* Creates a copy of target list.
*
* The new copy will contain all the elements from the source list in the same
* order and will use the same functions as the original list for copying and
* freeing elements.
*
* @param list The target list to copy, copy functions
* @return
* NULL if a NULL was sent or a memory allocation failed.
* A List containing the same elements with same order as list otherwise.
*/
Node listCopy(Node list,copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement);

/**
 * Returns next node in a given list
 */
Node listGetNext(Node list);

/**
* Returns the number of elements in a list
*
* @param list The target list which size is requested.
* @return
* -1 if a NULL pointer was sent.
* Otherwise the number of elements in the list.
*/
int listGetSize(Node list);

/**
* Removes the currently pointed element of the list using the stored freeing
* function
*
* @param list The list for which the current element will be removed
* @return
* LIST_NULL_ARGUMENT if list is NULL
* LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
* LIST_SUCCESS the current element was removed successfully
*/
Node listRemoveCurrent(Node list,Node node, freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement, compareMapKeyElements compareMapKeyElements);

/**
* listDestroy: Deallocates an existing list. Clears all elements by using the
* stored free function.
*
* @param list Target list to be deallocated. If list is NULL nothing will be
* done
*/
void listDestroy(Node list, freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement);

//keyGetNode gets key and list and returns the node that contains this key
Node keyGetNode(MapKeyElement key, Node list, compareMapKeyElements compareKeyElements);

/*
 * listInsertElement: Inserts an element to existing list. locating the element in the list by his key
*/
Node listInsertElement(Node list , MapDataElement data , MapKeyElement Key , compareMapKeyElements compareKeyElements, copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement);

//return pointer to the data of a node
MapKeyElement listGetData(Node node);

//return pointer to the key of a node
MapKeyElement listGetKey(Node node);

//sets data for given node
void listSetData(Node node, MapDataElement data, copyMapDataElements copyDataElement \
		,freeMapDataElements freeDataElement);

#endif //HW3_LIST_MTM_H
