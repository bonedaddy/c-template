#include <stdlib.h>
#include <stdbool.h>
#ifdef _HAVE_SAFE_MEM_H
#include "../../include/utils/safe_mem.h"

/*
    free_memory_object can be used to safely free up allocated memory.
    the first time you call free_memory_object the memory is freed and 
        the void pointer is set to a null pointer
    the second time you call free_memory_object a -1 is returned
*/
int free_memory_object(memory_object *obj) {
    // check to see if we have freed the memory before
    if (obj->freed) {
        // return -1 indicating error
        return -1;
    }
    // set freed to true
    obj->freed = true;
    // deallocate the memory returning it to os
    free(obj->data);
    // set the data to a null pointer
    obj->data = NULL;
    return 0;
}

/*
    new_memory_object is used to return an initialized memory_object
    with the data member set to the void pointer
*/
memory_object new_memory_object(void *input) {
    memory_object mobj;
    mobj.freed = false;
    mobj.data = input;
    return mobj;
}