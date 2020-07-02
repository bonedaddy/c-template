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

int main(void) {
    new_logger_config("logger.conf");
    logger *loggr = new_logger("logger.conf", "file_debug");
    if (loggr == NULL) {
        printf("failed to get new logger");
        return -1;
    }
    zlog_info(loggr->z, "hello zlog");
    close_logger();
    return 0; 

} 