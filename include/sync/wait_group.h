/*! @file wait_group.h
  * @author Bonedaddy
  * @brief provides golang like sync.Wait functionality
  * allows management of multiple threads, and enabling clean exits or cleanups
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

/*! @typedef wait_group_t
  * @brief a simple structure of a pthread mutex and integer counter
*/
typedef struct wait_group_t {
    int active_processes;
    pthread_mutex_t mutex;
} wait_group_t;

/*! @brief incremements the total number of active processes managed by this wait group
  * @param wg the waitgroup to manipulate
  * @param count the number of new active processes
*/
void wait_group_add(wait_group_t *wg, int count);
/*! @brief used by a process to indicate it is done, decreasing the active process counter
  * we include a return code here to catch errors when trying to decrement count below 0
  * @pre if active_process is less than or equal to 0
  * @param wg the waitgroup to manipulate a runtime error will occur and program will exit
  * @return -1 on error 0 on success
*/
int wait_group_done(wait_group_t *wg);
/*! @brief used to wait until current active_processes reaches 0, polling every 0.75 seconds
  * will run free on the wait_group_t pointer to clear up resources
  * @param wg the waitgroup to wait on
*/
void wait_group_wait(wait_group_t *wg);
/*! @brief returns a new and initialized wait_group_t pointer
*/
wait_group_t *wait_group_new();
