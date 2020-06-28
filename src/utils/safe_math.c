#include <limits.h>
#include <stdbool.h>
#include "utils/safe_math.h"

int safe_add_int(int x, int y, bool *passed) {
    if (x > INT_MAX - y) {
        *passed = false;
        return -1;
    }
    *passed = true;
    return x + y;
}

// subtracts y from x (this might be incorrect)
int safe_sub_int(int x, int y, bool *passed) {
    if (x - y > x) {
        *passed = false;
        return -1;
    }
    *passed = true;
    return x - y;
}

// divides x by y
int safe_div_int(int x, int y, bool *passed) {
    if (y == 0) {
        *passed = false;
        return -1;
    }
    *passed = true;
    return x / y;
}

// modulo x by y
int safe_mod_int(int x, int y, bool *passed) {
    if (y == 0) {
        *passed = false;
        return -1;
    }
    *passed = true;
    return x % y;
}
// subtracts y from x
unsigned int safe_sub_uint(unsigned int x, unsigned int y, bool *passed) {
    if (y > x) {
        *passed = false;
        return 0;
    }
    *passed = true;
    return x - y;
}

// adds x to y
unsigned int safe_add_uint(unsigned int x, unsigned int y, bool *passed) {
    if (x + y < x || x + y < y) {
        *passed = false;
        return 0;
    }
    *passed = true;
    return x + y;
}

// divides x by y
unsigned int safe_div_uint(unsigned int x, unsigned int y, bool *passed) {
    if (y == 0) {
        *passed = false;
        return 0;
    }
    *passed = true;
    return x / y;
}

// modulo x by y
unsigned int safe_mod_uint(unsigned int x, unsigned int y, bool *passed) {
    if (y == 0) {
        *passed = false;
        return 0;
    }
    *passed = true;
    return x % y;
}

// multiply x by y
unsigned int safe_mul_uint(unsigned int x, unsigned int y, bool *passed) {
    if (x == 0 || y == 0) {
        *passed = true;
        return 0;
    }
    unsigned int z = x * y;
    if (z < x || z < y) {
        *passed = false;
        return 0;
    }
    *passed = true;
    return z;
}