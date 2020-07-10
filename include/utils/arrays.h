/*! @file array.h
  * @brief array related helpers
  * includes array length and size helpers, as well as thread safe arrays
*/

#include <pthread.h>

// returns the number of elements in an array
#define array_len(x) ((array_size(x) > 0) ? sizeof(x) / sizeof(x[0]) : 0)
// returns the size of the array in bytes
#define array_size(x) (sizeof(x))

/*! @typedef iter_func is executed against each value found during iteration
  * @struct iter_func is executed against each value found during iteration
  * @param p a void pointer to the value found, although it can be anything else such as a struct
*/
typedef void (iter_func)(void *p);

/*! @typedef int_array is a thread safe array of integer values with helper functions
  * @struct  int_array is a thread safe array of integer values with helper functions
  * @brief provides convenience functions for managing the array (pop, append, etc...)
*/
typedef struct int_array {
  int *values;
  int count;
  int max;
  pthread_mutex_t mutex;
} int_array;

/*! @brief returns a new int_array with a max capacity of `max`
  * max isn't a hard limit, it simply defines the amount of memory to preallocate
  * if you append to the array and there is not enough space, the array size will be doubled
*/
int_array *new_int_array(int max);

/*! @brief an example iteration func
*/
void print_iter_func(void *p);

/*! @brief appens value to the end of the values array
  * @note will resize array if not enough room by 2x
*/
void append_int_array(int_array *arr, int value);

/*! @brief removes the last element off the array and returns it
  * @note decreases arr::count
*/
int pop_int_array(int_array *arr);

/*! @brief returns the number of elements in the array
*/
int length_int_array(int_array *arr);

/*! @brief iterates over all values in the array
  * each value in the array has iter_fn executed against it
*/
void iter_int_array(int_array *arr, iter_func iter_fn);