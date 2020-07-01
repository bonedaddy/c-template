#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "command_line.c"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_is_flag_argument(void **state) {
    char *arg = "--config";
    bool is_flag = is_flag_argument(arg);
    assert(is_flag == true);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_is_flag_argument),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}