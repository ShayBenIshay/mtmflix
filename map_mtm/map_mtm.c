//
//	map_mtm.c
// Created by:
//	Shay Ben-Ishay 308052463
//	Liav Eliyahu 311502660
//
#include "map_mtm.h"
#include "List_mtm.h"
#include <stdlib.h>
#include <stdio.h>

struct Map_t {
    Node list;
    int mapSize;
    MapKeyElement iterator;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

//internal functions//

static void mapPrint(Map map){
    if (!map)   return;
    int count=0;
    printf("\nmap has %d nodes:\n",map->mapSize);
    Node iterator_node = keyGetNode(map->iterator,map->list,map->compareKeyElements);
    if (iterator_node==NULL){
        printf("Iterator is: NULL\n");
    } else{
        printf("Iterator is: Key:%d Data:%d\n",*(int*)map->iterator,*(int*)listGetData(iterator_node));
    }
    MAP_FOREACH(MapKeyElement,iterator,map){
        Node currentNode = keyGetNode(iterator,map->list,map->compareKeyElements);
        printf("Element No.%d: Key:%d Data:%d\n",count++,*(int*)listGetKey(currentNode),*(int*)listGetData(currentNode));
    }
    return;
}

//end of internal functions//


//mapCreate: Allocates a new empty map.
Map mapCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){
    Map map = malloc(sizeof(*map));
    if (!map || !copyDataElement || !copyKeyElement || !freeDataElement ||\
     !freeKeyElement || !compareKeyElements)	return NULL;  //ALLOCATE ERROR - return relevant message
    map->list = listCreate();
    if(!map->list){
        free(map);
        return NULL;
    }
    //if NULL its allocate error --> should or should'nt check if NULL
    map->mapSize=0;
    map->iterator=listGetKey(map->list);
    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;
    return map;
}

// mapDestroy: Deallocates an existing map. Clears all elements by using the
void mapDestroy(Map map){
    if (!map) return;
    listDestroy(map->list,map->freeDataElement,map->freeKeyElement);
    free(map);
    return;
}

// mapGetSize: returns the size of the map
int mapGetSize(Map map){
    if (!map) return -1;
    return map->mapSize;
}

// mapContains: returns true if element exists in map, false otherwise.
bool mapContains(Map map, MapKeyElement element){
    if (!map || !element) return false;
    Node map_list_ptr = map->list;
        while (map_list_ptr!=NULL && listGetKey(map_list_ptr)!=NULL) {
            if (map->compareKeyElements(element,listGetKey(map_list_ptr))==0) {
                return true;
            }
            map_list_ptr=listGetNext(map_list_ptr);
        }
    return false;
}

// mapCopy: returns a copy to a given map.
Map mapCopy(Map map){
    if (!map) return NULL;//ALLOCATE ERROR
    Map map_copied = mapCreate(map->copyDataElement,map->copyKeyElement,map->freeDataElement,\
    map->freeKeyElement,map->compareKeyElements);
    if (!map_copied) return NULL;
    Node list_copied = listCopy(map->list,map->copyDataElement,map->copyKeyElement);
    if (!list_copied){
        mapDestroy(map_copied);
        return NULL;
    }
    map_copied->list = list_copied;
    map_copied->mapSize=map->mapSize;
    map_copied->iterator=map->iterator;
    map_copied->copyDataElement=map->copyDataElement;
    map_copied->copyKeyElement=map->copyKeyElement;
    map_copied->freeDataElement = map->freeDataElement;
    map_copied->freeKeyElement = map->freeKeyElement;
    map_copied->compareKeyElements=map->compareKeyElements;
   return map_copied;
}

// mapGetFirst: returns the first Key in a given map.
MapKeyElement mapGetFirst(Map map){
    if (!map) return NULL;
    map->iterator=listGetKey(map->list);
    return map->iterator;
}

// mapGetNext: returns the next key in a given map.
MapKeyElement mapGetNext(Map map){
    if (!map) return NULL;
    Node node_iterator = keyGetNode(map->iterator,map->list,map->compareKeyElements);
    node_iterator=listGetNext(node_iterator);
    map->iterator = listGetKey(node_iterator);
    return map->iterator;
}

// mapClear: clears the elements from a given map and returns status.
MapResult mapClear(Map map){
    if (!map) return MAP_NULL_ARGUMENT;
    listDestroy(map->list,map->freeDataElement,map->freeKeyElement);
    map->list = listCreate();
    if (!map->list) return MAP_OUT_OF_MEMORY;
    map->iterator = NULL;
    map->mapSize = 0;
    return MAP_SUCCESS;
}

// mapGet: returns data for a given key in map.
MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if (!map || !keyElement) return NULL;
    if (!mapContains(map,keyElement))   return NULL;

    Node node = keyGetNode(keyElement,map->list,map->compareKeyElements);
    if (!node) return NULL;
    return listGetData(node);
}

// mapRemove: removes a given element from map, returns status.
MapResult mapRemove(Map map, MapKeyElement keyElement){
	if (!map || !keyElement) return MAP_NULL_ARGUMENT;
	if (mapContains(map,keyElement)==false) {
	    return MAP_ITEM_DOES_NOT_EXIST;
	}
	Node node_to_remove = keyGetNode(keyElement, map->list, map->compareKeyElements);
	mapPrint(map);
	map->list = listRemoveCurrent(map->list, node_to_remove, map->freeDataElement, map->freeKeyElement,map->compareKeyElements);
	map->mapSize--;
	mapPrint(map);
	return MAP_SUCCESS;
}

// mapPut: adds a given key and data to a map. returns status.
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
	if (!map || !keyElement || !dataElement) {
	    return MAP_NULL_ARGUMENT;
	}
	//in case key exists in map and needs to be updated with new data
	if(mapContains(map, keyElement)){
		Node node_ptr = keyGetNode(keyElement,map->list,map->compareKeyElements);
		if (!node_ptr) {
		    return MAP_NULL_ARGUMENT;
		}
		listSetData(node_ptr, dataElement, map->copyDataElement, map->freeDataElement);
		map->iterator=NULL;
		return MAP_SUCCESS;
	} else {
		//in case key does not exist in map and needs to be added
        map->list = listInsertElement(map->list, dataElement, keyElement, \
				map->compareKeyElements, map->copyDataElement, map->copyKeyElement);
		if (map->list!=NULL) {
			map->mapSize++;
			map->iterator=NULL;
			//mapGetNext(map);
			return MAP_SUCCESS;
		}
		if (!map->list) return MAP_NULL_ARGUMENT;
		return MAP_SUCCESS;
	}
}

// ################ end of implementations ###################



