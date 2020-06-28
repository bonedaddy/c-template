#include <stdlib.h>
#include <assert.h>
#include "utils/safe_mem.c"

int main(void) {
    memory_object obj = new_memory_object(NULL);
    assert(free_memory_object(&obj) == 0);
}