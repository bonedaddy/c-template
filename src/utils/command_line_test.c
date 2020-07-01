#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "command_line.c"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_print_help(void **state) {
    print_help();
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_print_help),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}