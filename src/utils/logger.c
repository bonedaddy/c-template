#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils/logger.h"

logger *new_logger(char *config_path, char *category) {
    // open the config file
    int return_code = zlog_init(config_path);
    if (return_code) {
        printf("failed to initialize zlog\n");
        zlog_fini();
        return NULL;
    }
    // open the log category which is a particular
    // configuration of zlog
    zlog_category_t *zl = zlog_get_category(category);
    if (!zl) {
        printf("failed to open zlog category");
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
    const char *default_config =    "[formats]\n"
                                    "simples = \"%m%n\"\n"
                                    "[rules]\n"
                                    "simple_debug.DEBUG >stdout; simple\n";
    FILE *file_handler = fopen(config_path, "w");
    fputs(default_config, file_handler);
    return fclose(file_handler);
}

void close_logger(void) {
    zlog_fini();
}

int main(void) {
    new_logger_config("logger.conf");
    logger *loggr = new_logger("test_hello.conf", "my_cat");
    if (loggr == NULL) {
        printf("failed to get new logger");
        return -1;
    }
    zlog_info(loggr->z, "hello zlog");
    zlog_fini();
    return 0; 

} 