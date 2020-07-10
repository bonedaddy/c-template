#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../../include/sync/wait_group.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_wait_group(void **state) {
    wait_group_t *wg = wait_group_new();
    wait_group_add(wg, 1);
    assert(wg->active_processes == 1);
    wait_group_add(wg, 2);
    assert(wg->active_processes == 3);
    wait_group_done(wg);
    assert(wg->active_processes == 2);
    wait_group_done(wg);
    assert(wg->active_processes == 1);
    wait_group_done(wg);
    assert(wg->active_processes == 0);
    wait_group_wait(wg);
    int rc = wait_group_done(wg);
    assert(rc == -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_wait_group),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}