/*! @file wait_group.h
  * @author Bonedaddy
  * @brief provides C equivalent of Golang sync.Wait
  * enables "signalling" multiple threads
  * allows management of multiple threads, and enabling clean exits or cleanups
  * it is a bit different than joining pthreads to wait for processes to exit
  * as it doesn't require attached pthreads (can be used with detached) for synchronizatiion
  * @todo:
  *   - determine if this is more efficient than using thread joining
  *   - expand signalling capabilities to enable a channel
*/

#pragma once

#include <stdio.h>
#include <pthread.h>

/*! @typedef wait_group_t
  * @brief a simple structure of a pthread mutex and integer counter
*/
typedef struct wait_group_t {
    int active_processes;
    pthread_mutex_t mutex;
    pthread_mutex_t cond_mutex;
    pthread_cond_t cond_var;
} wait_group_t;

/*! @brief incremements the total number of active processes managed by this wait group
  * @param wg the waitgroup to manipulate
  * @param count the number of new active processes
*/
void wait_group_add(wait_group_t *wg, int count);

/*! @brief used by a process to indicate it is done, decreasing the active process counter
  * we include a return code here to catch errors when trying to decrement count below 0
  * @pre wg->active_process must be greater than or equal to 1 otherwise return -1
  * @warning in the future this may be changed so that calling wait_group_done with a 0 value active_processes will cause a program exit
  * @param wg the waitgroup to manipulate a runtime error will occur and program will exit
  * @return Failure: -1
  * @return Success: 0
  * @todo:
  *    - (bonedaddy) decide if we should do a runtime exit on pre condition failure
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


void wait_group_recv_signal(wait_group_t *wg);

void wait_group_listen_signal(wait_group_t *wg);
void wait_group_send_signal(wait_group_t *wg);