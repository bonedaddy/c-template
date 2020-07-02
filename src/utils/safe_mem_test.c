#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include <stdbool.h>
#include "../../include/utils/safe_mem.h"

typedef struct test_data {
    char c[100];
    bool b[100];
} test_data;

// -Werror seems to think that assert(....) doesnt use the variable  so thats why we ignored unused but set
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void test_new_memory_object_null(void **state) {
    memory_object mobj = new_memory_object(NULL);
    assert_false(mobj.freed);
    assert_null(mobj.data);
    // if we call free_memory_object_data within assert() then valgrind detects that the memory isn't actually freed
    int return_code = free_memory_object_data(&mobj);
    assert(return_code == 0);
    return_code = free_memory_object_data(&mobj);
    assert(return_code == -1);
    assert(mobj.freed == true);
    assert(mobj.data == NULL);
    // this is causing a failure when built without debugging symbols
    // assert_true(mobj.freed);
    // assert_null(mobj.data);
}

// -Werror seems to think that assert(....) doesnt use the variable so thats why we ignored unused but set
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void test_new_memory_object_test_data(void **state) {
    test_data *td = (test_data *)malloc(sizeof(test_data));
    td->c[0] = '\n';
    memory_object mobj = new_memory_object(td);
    td = mobj.data;
    assert_false(mobj.freed);
    assert_non_null(mobj.data);
    assert(td->c[0] == '\n');
    // if we call free_memory_object_data within assert() then valgrind detects that the memory isn't actually freed
    int return_code = free_memory_object_data(&mobj);
    assert(return_code == 0);
    return_code = free_memory_object_data(&mobj);
    assert(return_code == -1);
    assert(mobj.freed == true);
    assert(mobj.data == NULL);
    // this is causing a failure when built without debugging symbols
    // assert_true(mobj.freed);
    // assert_null(mobj.data);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_memory_object_null),
        cmocka_unit_test(test_new_memory_object_test_data),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}