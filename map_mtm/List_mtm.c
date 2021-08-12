//
//	List_mtm.c
// Created by:
//	Shay Ben-Ishay 308052463
//	Liav Eliyahu 311502660
//
#include "map_mtm.h"
#include "List_mtm.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct List{
    MapDataElement data;
    MapKeyElement key;
    Node next;
 };

/*
 * internal functions signature
*/
static MapKeyElement listGetNextKey(Node node);
/*
 * end of internal functions signatures
 */

//return pointer to the data of a node
MapDataElement listGetData(Node node){
    if (!node)  return NULL;
    return node->data;
}

//return pointer to the key of a node
MapKeyElement listGetKey(Node node){
    if (!node)  return NULL;
    return node->key;
}

//sets data for given node
void listSetData(Node list, MapDataElement data, copyMapDataElements copyDataElement \
		,freeMapDataElements freeDataElement){
    assert(list || data || freeDataElement);
    freeDataElement(list->data);
    list->data = copyDataElement(data);
    return;
}

//Creating a new List and returns the first Node
Node listCreate() {
    Node list = malloc(sizeof(*list));
    if (!list)   return NULL; //ALLOCATE ERROR - return relevant message
    list->data = NULL;
    list->key = NULL;
    list->next = NULL;
    return list;
}

Node listGetNext(Node list){
    if ( !list ) return NULL;
    return list->next;
}

void listDestroy(Node list , freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement){
    //Node node_temp = malloc(sizeof(*node_temp));
    if (!list || !freeDataElement || !freeKeyElement) return;
    Node list_ptr = list;
    Node list_ptr_next = listGetNext(list);
    while(list_ptr_next!=NULL) {
        freeDataElement(list_ptr->data);
        freeKeyElement(list_ptr->key);
        Node list_ptr_tmp = list_ptr;
        list_ptr = listGetNext(list_ptr);
        list_ptr_next = listGetNext(list_ptr);
        free(list_ptr_tmp);
    }
    freeDataElement(list_ptr->data);
    freeKeyElement(list_ptr->key);
    free(list_ptr);
    return;
}

int listGetSize(Node list){
    if (!list) return 0;
    Node node_p = list;
    int nodes_counter=0;
    do{
        node_p = listGetNext(node_p);
        nodes_counter++;
    }while(node_p!=NULL);
    return nodes_counter;
}

Node listInsertElement(Node list, MapDataElement data , MapKeyElement key , compareMapKeyElements\
 compareKeyElements, copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement){
    if (!list || !data || !key || !compareKeyElements || !copyDataElement || !copyKeyElement) {
        return NULL;
    }

    if(!list->key){
        //insert the node to the first element in list
        list->key = copyKeyElement(key);
        list->data = copyDataElement(data);
        return list;
    }
    //Create new Node and insert it in correct order
    Node node = listCreate();
    if (!node)   return NULL; //ALLOCATE ERROR - return relevant message
    node->key = copyKeyElement(key);
    node->data = copyDataElement(data);
    if(!node->key || !node->data) return NULL;

    Node node_pointer = list;
    MapKeyElement key_next = listGetNextKey(node_pointer) ;
    while( key_next != NULL ) {
        if(compareKeyElements(key_next,key) > 0 ) {
            //found the the location to insert the node
            MapKeyElement cur_key = listGetKey(node_pointer);
            //need to understand, if to add before or after ptr
            if (compareKeyElements(cur_key,key)>0){
                //add before ptr
                node->next=node_pointer;
                list = node;
            } else {
                //add after ptr
                node->next=keyGetNode(key_next,list,compareKeyElements);
                node_pointer->next=node;
            }
            return list;
        } else {
            //continue
            node_pointer = listGetNext(node_pointer);
            key_next = listGetNextKey(node_pointer);
        }
    }
    MapKeyElement cur_key = listGetKey(node_pointer);
    if(compareKeyElements(cur_key,key) < 0 ){
        node_pointer->next = node;
    } else {
        node->next=node_pointer;
        list = node;
    }
    return list;
}

Node listCopy(Node list, copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement){
    if(!list || !copyDataElement || !copyKeyElement) return NULL;
    int size = listGetSize(list);
    Node list_copy = listCreate();
    if (!list_copy) return NULL;
    Node node_copy_temp = list_copy;
    Node node_temp = list;
    for(int i=0;i<size;i++){
        node_copy_temp->key = copyKeyElement(node_temp->key);
        node_copy_temp->data = copyDataElement(node_temp->data);
        node_temp = listGetNext(node_temp);
        if(!node_temp) break;
        node_copy_temp->next = listCreate();
        if(!node_copy_temp->next) return NULL; // allocate error
        node_copy_temp = listGetNext(node_copy_temp);
    }
    return list_copy;
}

Node listRemoveCurrent(Node list,Node node, freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement, compareMapKeyElements compareMapKeyElements) {
    if (!list || !node) return NULL;
    //if node == list => remove first node
    if (compareMapKeyElements(listGetKey(list),listGetKey(node))) {
        //in case need to remove the first node
        MapKeyElement next_key=listGetNextKey(node);
        if (next_key==NULL){
            //if this is the only node in list
            listDestroy(node, freeDataElement, freeKeyElement);
        } else {
            //need to remove first node and keep list from second node
            list = node->next;
            node->next = NULL;
            listDestroy(node, freeDataElement, freeKeyElement);
        }
        return list;
    }
    //in case node is not the first in list, find the position
    Node node_ptr = list;
    MapKeyElement node_key = listGetKey(node);
    Node node_ptr_next = listGetNext(node_ptr);
    while (node_ptr_next != NULL) {
        if (compareMapKeyElements(listGetKey(node_ptr_next),node_key)) {
            //found key to be removed
            if (listGetNext(node_ptr_next)==NULL){
                //in case this is the last node in list
                node_ptr->next=NULL;
                listDestroy(node_ptr_next,freeDataElement,freeKeyElement);
            } else {
                //in case next is not last in list
                node_ptr->next=listGetNext(node_ptr_next);
                listDestroy(node_ptr_next,freeDataElement,freeKeyElement);
            }
            return list;
        } else {
            //not found key to be removed, skip
            node_ptr = listGetNext(node_ptr);
            node_ptr_next = listGetNext(node_ptr);
        }
    }
    return NULL;
}

//keyGetNode gets key and list and returns the node that contains this key
Node keyGetNode(MapKeyElement key, Node list, compareMapKeyElements compareKeyElements){
    if (!key || !list || !compareKeyElements)   {
        return NULL;
    }
    Node node_ptr = list;
    while (node_ptr!=NULL){
        if (compareKeyElements(listGetKey(node_ptr),key)==0)   {
            return node_ptr;
        }
        node_ptr=listGetNext(node_ptr);
    }
    return NULL;
}

/*
 * ################ internal functions implementation ##########################
*/

//return MapKeyElement pointer to the key of the next node return NULL if node is last
static MapKeyElement listGetNextKey(Node node){
    assert(node!=NULL);
    if (node->next == NULL) return NULL;
    return node->next->key;
}

