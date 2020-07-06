#include "errors.h"

typedef struct {
    // value is the data returned by a function call (if any)
    void *value;
    // if not a NULL pointer than it means the function call errored
    // and the value struct member will be a NULL pointer
    cg_error *err;
} cg_return;

cg_return *new_cg_return(void *value, cg_error *err);