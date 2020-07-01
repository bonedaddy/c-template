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


int free_memory_object(memory_object *obj);
memory_object new_memory_object(void *input);