/*! @file socket_server.h
  * @author Bonedaddy
  * @brief TCP socket servers, clients, and tooling for working with sockets
  * @details uses wait_group.h to provide lightweight synchronization between pthreads
  * @warning before use you must call setup_signal_handling() so that all threads get properly cleaned up on exit
  * @note you will want to adjust `async_handle_conn_func` to suit your needs as right now it is just an echo client
  * it is likely you will need to have `#define _POSIX_C_SOURCE 201112L` 
  * see the following for more information
  *    - https://stackoverflow.com/questions/39409846/why-does-gcc-not-complain-about-htons-but-complains-about-getaddrinfo-when-c/39410095#39410095
  *    - https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/

#pragma once

#include <pthread.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// sys/time.h is needed for the timeval
//  #include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include "socket.h"
#include "../../utils/logger.h"
#include "../../sync/wait_group.h"
#include "../../utils/arrays.h"

/*! @typedef socket_server
  * @struct socket_server
  * @brief a generic tcp socket server using file descriptor socket_number
  * it uses a dedicated pthread for accepting new client connections, with each
  * connection being processed in another pthread in detached state.
  * socket uses SO_REUSEADDR
*/
typedef struct socket_server {
    int socket_number;
    log_fn log; // alis for thl->log
    log_fnf logf; // alias for thl->logf
    thread_logger *thl;
    pthread_t thread;
    // pthread_attr_t taddr;
    wait_group_t *wg;
    // array of active sockets
    int_array *client_socket_numbers;
} socket_server;

/*! @typedef client_conn
  * @struct client_conn
  * @brief a structure containing a file descriptor and address information
  * @todo
  *   - enable a queue/list of these
*/
typedef struct client_conn {
    int socket_number;
    sock_addr_storage *address;
} client_conn;

/*! @typedef conn_handle_data
  * @struct conn_handle_data
  * @brief struct containing arguments passed into pthread
*/
typedef struct conn_handle_data {
    pthread_t thread;
    socket_server *srv;
    client_conn *conn; 
} conn_handle_data;

/*! @brief returns a new socket server bound to the port number and ready to accept connections
*/
socket_server *new_socket_server(addr_info hints, thread_logger *thl, int max_conns, char *port);

/*! @brief listens for new connections and spawns a thread to process the connection
  * thread that is created to process the connection runs as a detached thread
  * will poll for new connections to accept every 500 miliseconds
  * @param data void pointer to a socket_server struct
  * @note detached thread created calling async_handle_conn_func
  * @warning may change the 500 milisecond sleep
*/
void *async_listen_func(void *data);

/*! @brief handles connections in a dedicated pthread   
  * is laucnched in a pthread by async_listen_func when any new connection is received
  * @param data `void *` to a conn_handle_data object
  * @note uses `select` to determine if we can read data from the connection
  * @note select runs for 3 seconds before timing out and releasing resources with the connection
  * @warning currently implements an example echo client
  * @warning you will want to adapt to your specific use case
*/
void *async_handle_conn_func(void *data);

/*! @brief helper function for accepting client connections
  * times out new attempts if they take 3 seconds or more
  * @return Failure: NULL client conn failed
  * @return Success: non-NULL populated client_conn object
*/
client_conn *accept_client_conn(socket_server *srv);