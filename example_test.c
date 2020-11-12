#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "example.c"

#pragma GCC diagnostic ignored "-Wunused-parameter"

void test_return_one(void **state) {
    assert(return_one == 1);
}

void test_return_two(void **state) {
    assert(return_two == 1);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_return_one),
        cmocka_unit_test(test_return_two),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}