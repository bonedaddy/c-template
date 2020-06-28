#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
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

void *get_data_memory_object(memory_object *obj) {
    // try locking otherwise return null pointer
    int status = pthread_mutex_trylock(obj->mutex);
    if (!status) {
        return NULL;
    }
    void *pdata = atomic_load(&obj->data);
    atomic_store(&obj->locked, true);
    return pdata;
}

int put_data_memory_object(memory_object *obj, void *data) {
    // if this check fails, it means get_data_memory_object wasnt called
    // and thus is an error, so return -1
    if (!atomic_load(&obj->locked)) {
        return -1;
    }
    atomic_store(&obj->data, data);
    atomic_store(&obj->locked, false);
    pthread_mutex_unlock(obj->mutex);
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
    mobj.mutex = NULL;
    pthread_mutex_init(mobj.mutex, NULL);
    return mobj;
}