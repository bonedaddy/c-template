/*! @file logger.h
  * @brief provides logging related functionality
  * provides a thread safe logger capable of printing and writing colored logs, as well as a deprecated wrapper around zlog
*/

#pragma once

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "colors.h"
#include "zlog.h"

/*! @brief base struct used by the thread_logger
*/
struct thread_logger;

/*! @brief specifies log_levels, typically used when determining function invocation by log_fn
*/
typedef enum { LOG_LEVELS_INFO, LOG_LEVELS_WARN, LOG_LEVELS_ERROR, LOG_LEVELS_DEBUG } LOG_LEVELS;

/*! @brief signature of pthread_mutex_unlock and pthread_mutex_lock used by the thread_logger
*/
typedef int (*mutex_fn)(pthread_mutex_t *mx);

/*! @brief signature used by the thread_logger for log_fn calls
*/
typedef void (*log_func)(struct thread_logger *thl, char *message, LOG_LEVELS level);

/*! @struct a thread safe logger
  * @brief guards all log calls with a mutex lock/unlock
  * TODO(bonedaddy): enable output to a file
*/
typedef struct thread_logger {
    // todo(bonedaddy): make atomic
    bool debug;
    pthread_mutex_t mutex;
    mutex_fn lock;
    mutex_fn unlock;
    log_func log_fn;
} thread_logger;

/*! @brief returns a new thread safe logger
  * if with_debug is false, then all debug_log calls will be ignored
*/
thread_logger *new_thread_logger(bool with_debug);
/*! @brief main function you should call, which will delegate to the appopriate *_log function
*/
void log_fn(thread_logger *thl, char *message, LOG_LEVELS level);

/*! @brief logs a debug styled message - called by log_fn
*/
void debug_log(thread_logger *thl, char *message);

/*! @brief logs a warned styled message - called by log_fn
*/
void warn_log(thread_logger *thl, char *message);
/*! @brief logs an error styled message - called by log_fn
*/
void error_log(thread_logger *thl, char *message);
/*! @brief logs an info styled message - called by log_fn
*/
void info_log(thread_logger *thl, char *message);

/*! @brief logger is a wrapper around zlog
*/
typedef struct logger {
    zlog_category_t *z;
} logger;

/*! @brief initializes logger from the zlog config and category
*/
logger *new_logger(char *config_path, char *category);

/*! @brief writes a default zlog config to config_path
*/
int new_logger_config(char *config_path);

/*! @brief used to cleanup resources associated with zlog
*/
void close_logger(void);