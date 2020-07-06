#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/errors.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_new_cg_error(void **state) {
    char *msg = "hello world";
    cg_error *err = new_cg_error(msg);
    assert(
        strcmp(err->message, msg) == 0
    );
    free_cg_error(err);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_cg_error_string(void **state) {
    char *msg = "hello world";
    char *errstr = cg_error_string(msg);
    assert(
        strcmp(errstr, "hello world") == 0
    );
    free(errstr);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_wrap_cg_error(void **state) {
    cg_error *err = new_cg_error("part1");
    int response = wrap_cg_error(err, "part2");
    assert(response == 0);
    assert(
        strcmp(err->message, "part1 | part2") == 0
    );
    free_cg_error(err);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_cg_error),
        cmocka_unit_test(test_cg_error_string),
        cmocka_unit_test(test_wrap_cg_error),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
