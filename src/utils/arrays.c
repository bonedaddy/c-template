#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../../include/utils/arrays.h"

/*! @brief returns a new int_array with a max capacity of `max`
  * max isn't a hard limit, it simply defines the amount of memory to preallocate
  * if you append to the array and there is not enough space, the array size will be doubled
*/
int_array *new_int_array(int max) {
    if (max == 0) {
        max = 10;
    }
    int_array *arr = malloc(sizeof(int_array) + sizeof(int) * max);
    arr->values = calloc(sizeof(int), max);
    arr->count = 0;
    arr->max = max;
    pthread_mutex_init(&arr->mutex, NULL);
    return arr;
}

/*! @brief appens value to the end of the values array
  * @note will resize array if not enough room by 2x
*/
void append_int_array(int_array *arr, int value) {
    pthread_mutex_lock(&arr->mutex);
    if (arr->count + 1 >= arr->max) {
        // increase available space by 2x
        arr->values = realloc(
                // re allocate the size of the current array + arr->max * 2
                arr->values, sizeof(arr->values) * (sizeof(int) * (arr->max * 2))
        );
        arr->max *= 2;
    }
    // set the current count number as the given value
    arr->values[arr->count] = value;
    // increment count which will give us the index for the next append
    arr->count++;
    pthread_mutex_unlock(&arr->mutex);
}

/*! @brief removes the last element off the array and returns it
  * @note decreases arr::count
*/
int pop_int_array(int_array *arr) {
    int ret = -1;
    pthread_mutex_lock(&arr->mutex);
    if (arr->count == 0) {
        goto DO_UNLOCK;
    }
    // arrays are offset by 1
    ret = arr->values[arr->count - 1];
    // decrease count
    arr->count--;
    DO_UNLOCK:
        pthread_mutex_unlock(&arr->mutex);
    return ret;
}

int peek_int_array(int_array *arr) {
    int ret = -1;
    pthread_mutex_lock(&arr->mutex);
    if (arr->count == 0) {
        goto DO_UNLOCK;
    }
    ret = arr->values[arr->count - 1];
    DO_UNLOCK:
        pthread_mutex_unlock(&arr->mutex);
    return ret;
}

/*! @brief returns the number of elements in the array
*/
int length_int_array(int_array *arr) {
    pthread_mutex_lock(&arr->mutex);
    int ret = arr->count;
    pthread_mutex_unlock(&arr->mutex);
    return ret;
}

/*! @brief an example iteration func
*/
void print_iter_func(void *p) {
    printf("%i\n", *(int *)p);
}

/*! @brief iterates over all values in the array
  * each value in the array has iter_fn executed against it
*/
void iter_int_array(int_array *arr, iter_func iter_fn) {
    pthread_mutex_lock(&arr->mutex);
    for (int i = 0; i < arr->count; i++) {
        int value = arr->values[i];
        // execute function on value
        iter_fn(&value);
    }
    pthread_mutex_unlock(&arr->mutex);
}