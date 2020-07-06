/*! @file errors.h
  * @brief provides a C equivlent of golang function returns
  * it enables returning two values from function call, one being an error (if any) and one being data if any
*/

#pragma once

#include "errors.h"
#include <stdlib.h>

/*! @brief allows returning both a value and error message from a function call
  * useful as it makes it easier to provid better error message and handling similar to go
*/
typedef struct {
    // value is the data returned by a function call (if any)
    void *value;
    // if not a NULL pointer than it means the function call errored
    // and the value struct member will be a NULL pointer
    cg_error *err;
} cg_return;

/*! @brief creates a new cg_return struct
*/
cg_return *new_cg_return(void *value, cg_error *err);
/*! @brief frees up resources associated with an instance of cg_return
*/
void free_cg_return(cg_return *ret);