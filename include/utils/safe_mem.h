#include <stdlib.h>
#include <stdbool.h>

/*
    memory_object is a wrapper around a void pointer type
    that pevents double-free vulnerabilities on that void pointer type
*/

typedef struct memory_object {
    void *data;
    bool freed;
} memory_object;


// frees the data field of memory_object but does not free memory allocated for the memory_object itself
int free_memory_object_data(memory_object *obj);
// returns a new memory_object with data field set to input
memory_object new_memory_object(void *input);