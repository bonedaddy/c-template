#include <stdio.h>
#include <stdlib.h>
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


void info_log_fn(thread_logger *thl, char *message) {
    thl->lock(&thl->mutex);
    info_log(message);
    thl->unlock(&thl->mutex);
}

void error_log_fn(thread_logger *thl, char *message) {
    thl->lock(&thl->mutex);
    error_log(message);
    thl->unlock(&thl->mutex);
}


void error_log(char *message) {
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = malloc(strlen(message) + strlen("[error]") + (size_t)2);
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
    free(msg);
}

void info_log(char *message) {
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = malloc(strlen(message) + strlen("[info]") + (size_t)2);
    msg[0] = '[';
    msg[1] = 'i';
    msg[2] = 'n';
    msg[3] = 'f';
    msg[4] = 'o';
    msg[5] = ']';
    msg[6] = ' ';
    strcat(msg, message);
    print_colored(COLORS_GREEN, msg);
    free(msg);
}

thread_logger *new_thread_logger() {
    thread_logger *thl = malloc(sizeof(thread_logger));
    thl->lock = fn_mutex_lock;
    thl->unlock = fn_mutex_unlock;
    thl->info_log = info_log_fn;
    thl->error_log = error_log_fn;
    pthread_mutex_init(&thl->mutex, NULL);
    return thl;
}

int main(void) {
    thread_logger *thl = new_thread_logger();
    thl->info_log(thl, "hello world");
    thl->error_log(thl, "hello world");
    new_logger_config("logger.conf");
    logger *loggr = new_logger("logger.conf", "file_debug");
    if (loggr == NULL) {
        printf("failed to get new logger");
        return -1;
    }
    zlog_info(loggr->z, "hello zlog");
    close_logger();
   // free(loggr);
    return 0; 

} 