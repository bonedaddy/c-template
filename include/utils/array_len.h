#ifndef _HAVE_ARRAY_LEN_H
#define _HAVE_ARRAY_LEN_H
// returns the number of elements in an array
#define array_len(x) (array_size(x) > 0) ? sizeof(x) / sizeof(x[0]) : 0
// returns the size of the array in bytes
#define array_size(x) sizeof(x)
#endif