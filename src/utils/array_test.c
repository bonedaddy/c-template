#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/array_len.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpedantic"
void test_array_len(void **state) {
    int empty[0];
    int one[1] = {0};
    assert(array_len(empty) == 0);
    assert(array_len(one) == 1);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpedantic"
void test_array_size(void **state) {
    int empty[0];
    int two[2] = {0, 1};
    assert(array_size(empty) == 0);
    assert(array_size(two) == sizeof(int)*2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_len),
        cmocka_unit_test(test_array_size),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
