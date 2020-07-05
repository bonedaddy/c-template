#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include <pthread.h>
#include "../../include/utils/logger.h"


void *test_log(void *data) {
    thread_logger *thl = (thread_logger *)data;
    thl->log(thl, "this is an info log", LOG_LEVELS_INFO);
    thl->log(thl, "this is a warn log", LOG_LEVELS_WARN);
    thl->log(thl, "this is an error log", LOG_LEVELS_ERROR);
    thl->log(thl, "this is a debug log", LOG_LEVELS_DEBUG);
    // commenting this out seems to get rid of memleaks reported by valgrind
    // pthread_exit(NULL);
    return NULL;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_thread_logger(void **state) {
    thread_logger *thl = new_thread_logger(true);
    thl->log(thl, "this is an info log", LOG_LEVELS_INFO);
    thl->log(thl, "this is a warn log", LOG_LEVELS_WARN);
    thl->log(thl, "this is an error log", LOG_LEVELS_ERROR);
    thl->log(thl, "this is a debug log", LOG_LEVELS_DEBUG);
    pthread_t threads[4];
    pthread_attr_t attrs[4];
    for (int i = 0; i < 4; i++) {
        pthread_attr_init(&attrs[i]);
        pthread_create(&threads[i], &attrs[i], test_log, thl);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }
    free(thl);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_thread_logger),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}