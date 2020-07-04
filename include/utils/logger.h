/* logger.h

    a wrapper around zlog.h with some convenience functions

    notes:
        - `#pragma once` will cause the compiler to only import this file once
*/

#include "zlog.h"
#include <pthread.h>
#include "colors.h"
#include <string.h>

struct thread_logger;

typedef void (*mutex_lock)(pthread_mutex_t *mx);
typedef void (*mutex_unlock)(pthread_mutex_t *mx);
typedef void (*log_func)(struct thread_logger *thl, char *message);

/*! @struct a thread safe logger
  * @brief guards all log calls with a mutex lock/unlock
*/
typedef struct thread_logger {
    // todo(bonedaddy): make atomic
    pthread_mutex_t mutex;
    mutex_lock lock;
    mutex_unlock unlock;
    log_func info_log;
    log_func error_log;
} thread_logger;


// logger is a wrapper around an instance of a zlog category
typedef struct logger {
    zlog_category_t *z;
} logger;

void error_log(char *message);
void error_log_fn(thread_logger *thl, char *message);
void info_log(char *message);
void info_log_fn(thread_logger *thl, char *message);
thread_logger *new_thread_logger();



void fn_mutex_lock(pthread_mutex_t *mx);
void fn_mutex_unlock(pthread_mutex_t *mx);

void fn_mutex_lock(pthread_mutex_t *mx) {
    pthread_mutex_lock(mx);
}

void fn_mutex_unlock(pthread_mutex_t *mx) {
    pthread_mutex_unlock(mx);
}

// initializes zlog
logger *new_logger(char *config_path, char *category);
// writes a sensible default configuration to config_path
int new_logger_config(char *config_path);
// wrapper around zlog_fini
void close_logger(void);