#include <limits.h>
#include <stdbool.h>

// flag should represent the *_MIN macro for vairous types
#define Abs(i, flag) ((i) >= 0 ? (i): ((i) == (flag) ? (flag) : -(i)))

int safe_add_int(int x, int y, bool *passed);
int safe_sub_int(int x, int y, bool *passed);
int safe_div_int(int x, int y, bool *passed);
int safe_mod_int(int x, int y, bool *passed);
unsigned int safe_sub_uint(unsigned int x, unsigned int y, bool *passed);
unsigned int safe_add_uint(unsigned int x, unsigned int y, bool *passed);
unsigned int safe_div_uint(unsigned int x, unsigned int y, bool *passed);
unsigned int safe_mod_uint(unsigned int x, unsigned int y, bool *passed);
unsigned int safe_mul_uint(unsigned int x, unsigned int y, bool *passed);

