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

void *test_thread_log(void *data) {
    thread_logger *thl = (thread_logger *)data;
    thl->log(thl, 0, "this is an info log", LOG_LEVELS_INFO);
    thl->logf(thl, 0, LOG_LEVELS_INFO, "%s\t%s", "one", "two");

    thl->log(thl, 0, "this is a warn log", LOG_LEVELS_WARN);
    thl->logf(thl, 0, LOG_LEVELS_WARN, "%s\t%s", "one", "two");

    thl->log(thl, 0, "this is an error log", LOG_LEVELS_ERROR);
    thl->logf(thl, 0, LOG_LEVELS_ERROR, "%s\t%s", "one", "two");

    thl->log(thl, 0, "this is a debug log", LOG_LEVELS_DEBUG);
    thl->logf(thl, 0, LOG_LEVELS_DEBUG, "%s\t%s", "one", "two");
    // commenting this out seems to get rid of memleaks reported by valgrind
    // pthread_exit(NULL);
    return NULL;
}

void *test_file_log(void *data) {
    file_logger *fhl = (file_logger *)data;
    fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is an info log", LOG_LEVELS_INFO);
    fhl->thl->logf(fhl->thl, fhl->file_descriptor, LOG_LEVELS_INFO, "%s\t%s", "one", "two");

    fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is a warn log", LOG_LEVELS_WARN);
    fhl->thl->logf(fhl->thl, fhl->file_descriptor, LOG_LEVELS_WARN, "%s\t%s", "one", "two");

    fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is an error log", LOG_LEVELS_ERROR);
    fhl->thl->logf(fhl->thl, fhl->file_descriptor, LOG_LEVELS_ERROR, "%s\t%s", "one", "two");
    
    fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is a debug log", LOG_LEVELS_DEBUG);
    fhl->thl->logf(fhl->thl, fhl->file_descriptor, LOG_LEVELS_DEBUG, "%s\t%s", "one", "two");
    // commenting this out seems to get rid of memleaks reported by valgrind
    // pthread_exit(NULL);
    return NULL;
}


#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_thread_logger(void **state) {
    bool args[2] = {false, true};
    for (int i = 0; i < 2; i++) {
        thread_logger *thl = new_thread_logger(args[i]);
        assert(thl != NULL);
        thl->log(thl, 0, "this is an info log", LOG_LEVELS_INFO);
        thl->log(thl, 0, "this is a warn log", LOG_LEVELS_WARN);
        thl->log(thl, 0, "this is an error log", LOG_LEVELS_ERROR);
        thl->log(thl, 0, "this is a debug log", LOG_LEVELS_DEBUG);
        pthread_t threads[4];
        pthread_attr_t attrs[4];
        for (int i = 0; i < 4; i++) {
            pthread_attr_init(&attrs[i]);
            pthread_create(&threads[i], &attrs[i], test_thread_log, thl);
        }
        for (int i = 0; i < 4; i++) {
            pthread_join(threads[i], NULL);
            pthread_attr_destroy(&attrs[i]);
        }
        clear_thread_logger(thl);
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_file_logger(void **state) {
    bool args[2] = {false, true};
    for (int i = 0; i < 2; i++) {
        file_logger *fhl = new_file_logger("file_logger_test.log", args[i]);
        assert(fhl != NULL);
        fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is an info log", LOG_LEVELS_INFO);
        fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is a warn log", LOG_LEVELS_WARN);
        fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is an error log", LOG_LEVELS_ERROR);
        fhl->thl->log(fhl->thl, fhl->file_descriptor, "this is a debug log", LOG_LEVELS_DEBUG);
        pthread_t threads[4];
        pthread_attr_t attrs[4];
        for (int i = 0; i < 4; i++) {
            pthread_attr_init(&attrs[i]);
            pthread_create(&threads[i], &attrs[i], test_file_log, fhl);
        }
        for (int i = 0; i < 4; i++) {
            pthread_join(threads[i], NULL);
            pthread_attr_destroy(&attrs[i]);
        }
        clear_file_logger(fhl);
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_thread_logger),
        cmocka_unit_test(test_file_logger),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}