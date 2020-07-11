#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/arrays.h"
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpedantic"
void test_array_len(void **state) {
    int one[1] = {0};
    int two[2] = {0, 1};
    assert(array_len(one) == 1);
    assert(array_len(two) == 2);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpedantic"
void test_array_size(void **state) {
    int one[1] = {0};
    int two[2] = {0, 1};
    assert(array_size(one) == sizeof(int));
    assert(array_size(two) == sizeof(int)*2);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_int_array(void **state) {
    int_array *arr = new_int_array(10);
    // test appends
    for (int i = 0; i < 10; i++) {
        append_int_array(arr, i);
        assert(length_int_array(arr) == i + 1);
        int top = peek_int_array(arr);
        assert(top == i);
    }
    // test print iter
    iter_int_array(arr, print_iter_func);
    for (int i = 9; i >= 0; i--) {
        int top = pop_int_array(arr);
        assert(top == i);
        assert(length_int_array(arr) == i);
    }
    for (int i = 0; i < 1000; i++) {
        append_int_array(arr, i);
    }
    free(arr->values);
    free(arr);
}


#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_len),
        cmocka_unit_test(test_array_size),
        cmocka_unit_test(test_int_array),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
