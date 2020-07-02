#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/random.h"
#include "../../include/utils/array_len.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_get_random_string_length(void **state) {
    char *word = get_random_string(10);
    assert(sizeof(word) == 8);
    free(word);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_get_random_number_range(void **state) {
    int number = get_random_number(0, 25);
    assert(number >= 0 && number <= 25);
}

int main(void) {
    // seed random number generate using time
    srand(time(0));
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_random_string_length),
        cmocka_unit_test(test_get_random_number_range),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
