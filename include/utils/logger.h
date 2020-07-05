/* logger.h

    a wrapper around zlog.h with some convenience functions

    notes:
        - `#pragma once` will cause the compiler to only import this file once
*/

#include "zlog.h"
#include <pthread.h>
#include "colors.h"
#include <string.h>
#include <stdbool.h>

struct thread_logger;

typedef enum { LOG_LEVELS_INFO, LOG_LEVELS_WARN, LOG_LEVELS_ERROR, LOG_LEVELS_DEBUG } LOG_LEVELS;
typedef int (*mutex_fn)(pthread_mutex_t *mx);
typedef void (*mutex_unlock)(pthread_mutex_t *mx);
typedef void (*log_func)(struct thread_logger *thl, char *message, LOG_LEVELS level);

/*! @struct a thread safe logger
  * @brief guards all log calls with a mutex lock/unlock
*/
typedef struct thread_logger {
    // todo(bonedaddy): make atomic
    bool debug;
    pthread_mutex_t mutex;
    mutex_fn lock;
    mutex_fnb unlock;
    log_func log_fn;
} thread_logger;


// logger is a wrapper around an instance of a zlog category
typedef struct logger {
    zlog_category_t *z;
} logger;

void debug_log(thread_logger *thl, char *message);
void warn_log(thread_logger *thl, char *message);
void error_log(thread_logger *thl, char *message);
void info_log(thread_logger *thl, char *message);
void log_fn(thread_logger *thl, char *message, LOG_LEVELS level);
thread_logger *new_thread_logger(bool with_debug);



void fn_mutex_lock(pthread_mutex_t *mx);
void fn_mutex_unlock(pthread_mutex_t *mx);



// initializes zlog
logger *new_logger(char *config_path, char *category);
// writes a sensible default configuration to config_path
int new_logger_config(char *config_path);
// wrapper around zlog_fini
void close_logger(void);