#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "../../include/utils/logger.h"

logger *new_logger(char *config_path, char *category) {
    // open the config file
    int return_code = zlog_init(config_path);
    if (return_code) {
        printf("failed to initialize zlog\n");
        close_logger();
        return NULL;
    }
    // open the log category which is a particular
    // configuration of zlog
    zlog_category_t *zl = zlog_get_category(category);
    if (!zl) {
        printf("failed to open zlog category");
        close_logger();
        return NULL;
    }
    // allocate memory size of `logger`, and the size of the returned cagetory
    // and then type cast to a `logger` pointer.
    // TODO(bonedaddy): not sure if we need to count for the size of `zl`
    logger *lg = (logger *)malloc(sizeof(logger)) + sizeof(zl);
    lg->z = zl;
    return lg;
}

int new_logger_config(char *config_path) {
    // strinct init - forces config file validation
    const char *default_config =    "[global]\n"
                                    "strict init = true\n"
                                    "rotate lock file = /tmp/zlog.lock\n"
                                    "[rules]\n"
                                    "simple_debug.DEBUG >stdout;\n"
                                    "simple_error.ERROR >stdout;\n"
                                    "file_debug.DEBUG \"/tmp/zlog.log\"\n"
                                    "file_error.ERROR \"/tmp/zlog.log\"\n";
    FILE *file_handler = fopen(config_path, "w");
    fputs(default_config, file_handler);
    return fclose(file_handler);
}

void close_logger(void) {
    zlog_fini();
}

void log_func(thread_logger *thl, char *message, LOG_LEVELS level) {
    switch (level) {
        case LOG_LEVELS_INFO:
            info_log(thl, message);
            break;
        case LOG_LEVELS_WARN:
            warn_log(thl, message);
            break;
        case LOG_LEVELS_ERROR:
            error_log(thl, message);
            break;
        case LOG_LEVELS_DEBUG:
            debug_log(thl, message);
            break;
    }
}

void info_log(thread_logger *thl, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[info]") + (size_t)2);
    msg[0] = '[';
    msg[1] = 'i';
    msg[2] = 'n';
    msg[3] = 'f';
    msg[4] = 'o';
    msg[5] = ']';
    msg[6] = ' ';
    strcat(msg, message);
    print_colored(COLORS_GREEN, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void warn_log(thread_logger *thl, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[warn]") + (size_t) 2);
    msg[0] = '[';
    msg[1] = 'w';
    msg[2] = 'a';
    msg[3] = 'r';
    msg[4] = 'n';
    msg[5] = ']';
    msg[6] = ' ';
    strcat(msg, message);
    print_colored(COLORS_YELLOW, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void error_log(thread_logger *thl, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[error]") + (size_t)2);
    msg[0] = '[';
    msg[1] = 'e';
    msg[2] = 'r';
    msg[3] = 'r';
    msg[4] = 'o';
    msg[5] = 'r';
    msg[6] = ']';
    msg[7] = ' ';
    strcat(msg, message);
    print_colored(COLORS_RED, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void debug_log(thread_logger *thl, char *message) {
    // unless debug enabled dont show
    if (thl->debug == false) {
        return;
    }
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[debug]") + (size_t) 2);
    msg[0] = '[';
    msg[1] = 'd';
    msg[2] = 'e';
    msg[3] = 'b';
    msg[4] = 'u';
    msg[5] = 'g';
    msg[6] = ']';
    msg[7] = ' ';
    strcat(msg, message);
    print_colored(COLORS_SOFT_RED, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

thread_logger *new_thread_logger(bool with_debug) {
    thread_logger *thl = malloc(sizeof(thread_logger));
    // thl->lock = fn_mutex_lock;
    thl->lock = pthread_mutex_lock;
    thl->unlock = pthread_mutex_unlock;
    thl->log = log_func;
    thl->debug = with_debug;
    pthread_mutex_init(&thl->mutex, NULL);
    return thl;
}