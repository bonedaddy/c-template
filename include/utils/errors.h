/*! @file errors.h
  * @brief golang like error handling
*/

#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>

/*! @struct wraps strings into a struct with error handling functionality
*/
typedef struct {
    char *message;
} cg_error;

/*! @brief returns a cg_error struct containing message
*/
cg_error *new_cg_error(char *message);
/*! @brief creates a new variable and moves memory of message into that variable returning it
  * this appears to be needed as if we dont do this `realloc` will complain about an invalid pointer
*/
char *cg_error_string(char *message);
/*! @brief appends message to the end of error->message
  * uses realloc to maximize memory efficiency
*/
int wrap_cg_error(cg_error *error, char *message);
/*! @brief frees up resources associated with error
*/
void free_cg_error(cg_error *error);