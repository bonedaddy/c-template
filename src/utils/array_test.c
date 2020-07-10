#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/arrays.h"

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

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
int main(void) {
    int_array *arr = new_int_array(10);
    assert(length_int_array(arr) == 0);
    append_int_array(arr, 100);
    assert(arr->values[arr->count - 1] == 100);
    assert(length_int_array(arr) == 1);
    append_int_array(arr, 200);
    iter_int_array(arr, print_iter_func);
    assert(length_int_array(arr) == 2);
    assert(arr->values[arr->count - 1] == 200);
    int top = pop_int_array(arr);
    assert(top == 200);
    assert(length_int_array(arr) == 1);
    top = pop_int_array(arr);
    assert(top == 100);
    assert(length_int_array(arr) == 0);
    for (int i = 0; i < 1000; i++) {
        append_int_array(arr, i);
    }
    printf("length: %i\n", length_int_array(arr));
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_len),
        cmocka_unit_test(test_array_size),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
