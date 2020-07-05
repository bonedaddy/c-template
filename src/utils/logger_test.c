#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/logger.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_thread_logger(void **state) {
    // TODO(bonedaddy): should we test with debug disabled?
    // TODO(bonedaddy): should we run tests using actual pthreads?
    thread_logger *thl = new_thread_logger(true);
    thl->log_fn(thl, "this is an info log", LOG_LEVELS_INFO);
    thl->log_fn(thl, "this is a warn log", LOG_LEVELS_WARN);
    thl->log_fn(thl, "this is an error log", LOG_LEVELS_ERROR);
    thl->log_fn(thl, "this is a debug log", LOG_LEVELS_DEBUG);
    exit_thread_logger(thl);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_pthread_logger(void **state) {
    // TODO(bonedaddy): should we test with debug disabled?
    // TODO(bonedaddy): should we run tests using actual pthreads?
    thread_logger *thl = new_thread_logger(true);
    p_log_fn(thl, "this is a pthread info log", LOG_LEVELS_INFO);
    p_log_fn(thl, "this is a pthread warn log", LOG_LEVELS_WARN);
    p_log_fn(thl, "this is a pthread error log", LOG_LEVELS_ERROR);
    p_log_fn(thl, "this is a pthread debug log", LOG_LEVELS_DEBUG);
    exit_thread_logger(thl);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_thread_logger),
        cmocka_unit_test(test_pthread_logger),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
    /* for zlog you can do the following
    new_logger_config("logger.conf");
    logger *loggr = new_logger("logger.conf", "file_debug");
    if (loggr == NULL) {
        printf("failed to get new logger");
        return -1;
    }
    zlog_info(loggr->z, "hello zlog");
    close_logger();
   // free(loggr);
    return 0; */
}