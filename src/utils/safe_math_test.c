#include <stdio.h>
#include <limits.h>
#include "../../include/utils/safe_math.h"

int main(void) {
    {
        bool passed;
        safe_sub_uint(99, 100, &passed);
        if (passed) {
            printf("bad check\n");
        }
        printf("success\n");
    }
    {
        bool passed;
        safe_sub_uint(100, 99, &passed);
        if (!passed) {
            printf("bad check\n");
        }
        printf("success\n");
    }
    {
        bool passed;
        safe_add_uint(UINT_MAX, UINT_MAX, &passed);
        if (passed) {
            printf("bad check\n");
        }
        printf("success\n");
    }
    printf("%ui\n", UINT_MAX + UINT_MAX);
}