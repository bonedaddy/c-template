#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

/*
    memory_object is a wrapper around a void pointer type
    that pevents double-free vulnerabilities on that void pointer type
*/

typedef struct memory_object {
    _Atomic void *data;
    bool freed;
    pthread_mutex_t *mutex;
    atomic_bool locked;
} memory_object;



int free_memory_object(memory_object *obj);
memory_object new_memory_object(void *input);
// retrieves the data object stored by memory_object
// this allows us to safely perform operations against it
// without worrying about concurrent access
void *get_data_memory_object(memory_object *obj);
// after modifying or operating with the data object
// return it to the memory_object and unlock the mutex
// allowing others to access it
int put_data_memory_object(memory_object *obj, void *data);