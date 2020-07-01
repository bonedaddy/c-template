/* logger.h

    a wrapper around zlog.h with some convenience functions

    notes:
        - `#pragma once` will cause the compiler to only import this file once
*/

#include "zlog.h"

// logger is a wrapper around an instance of a zlog category
typedef struct logger {
    zlog_category_t *z;
} logger;

// initializes zlog
logger *new_logger(char *config_path, char *category);
// writes a sensible default configuration to config_path
int new_logger_config(char *config_path);
// wrapper around zlog_fini
void close_logger(void);