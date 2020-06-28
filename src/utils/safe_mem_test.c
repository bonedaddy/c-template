#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include <stdbool.h>
#include "./include/utils/safe_mem.h"

typedef struct test_data {
    char c[100];
    bool b[100];
} test_data;

void test_new_memory_object_null(void **state) {
    memory_object mobj = new_memory_object(NULL);
    assert_false(mobj.freed);
    assert_null(mobj.data);
    assert(free_memory_object(&mobj) == 0);
    assert(free_memory_object(&mobj) == -1);
    assert_true(mobj.freed);
    assert_null(mobj.data);
}

void test_new_memory_object_test_data(void **state) {
    test_data *td = (test_data *)malloc(sizeof(test_data));
    td->c[0] = '\n';
    memory_object mobj = new_memory_object(td);
    td = mobj.data;
    assert_false(mobj.freed);
    assert_non_null(mobj.data);
    assert(td->c[0] == '\n');
    assert(free_memory_object(&mobj) == 0);
    assert(free_memory_object(&mobj) == -1);
    assert_true(mobj.freed);
    assert_null(mobj.data);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_memory_object_null),
        cmocka_unit_test(test_new_memory_object_test_data),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}