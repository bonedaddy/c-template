/*! @file socket.h
  * @author Bonedaddy
  * @brief a generic multi-threaded tcp socket server
*/

#pragma once

#include <pthread.h>
#include "../utils/logger.h"

/*! @typedef addr_info
  * @struct addrinfo
  * @brief alias for `struct addrinfo`
*/
typedef struct addrinfo addr_info;

/*! @typedef sock_addr
  * @struct sockaddr
  * @brief alias for `struct sockaddr`
*/
typedef struct sockaddr sock_addr;

/*! @typedef sock_addr_storage
  * @struct sockaddr_storage
  * @brief alias for `struct sockaddr_storage`
*/
typedef struct sockaddr_storage sock_addr_storage;


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
} socket_server;

/*! @typedef client_conn
  * @struct client_conn
  * @brief a structure containing a file descriptor and address information
  * @TODO:
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

typedef enum {
  REUSEADDR,
  NOBLOCK,
  BLOCK,
} SOCKET_OPTS;

/*! @brief generates an addr_info struct with defaults
  * defaults is IPv4, TCP, and AI_PASSIVE flags
*/
addr_info default_hints();

/*! @brief returns a new socket server bound to the port number and ready to accept connections
*/
socket_server *new_socket_server(addr_info hints, thread_logger *thl, int max_conns, char *port);

/*! @brief helper function for accepting client connections
  * @return Failure: NULL client conn failed
  * @return Success: non-NULL populated client_conn object
*/
client_conn *accept_client_conn(socket_server *srv);

/*! @brief  gets an available socket attached to bind_address
  * @return Success: file descriptor socket number greater than 0
  * @return Failure: -1
  * initializers a socket attached to bind_address with sock_opts, and binds the address
*/
int get_new_socket(thread_logger *thl, addr_info *bind_address, SOCKET_OPTS sock_opts[], int num_opts);


char  *get_name_info(sock_addr *client_address);
void *async_handle_conn_func(void *data);
void *async_listen_func(void *data);
void print_and_exit(int error_number);
void signal_handler_fn(int signal_number);
void setup_signal_handling();
bool set_socket_blocking_status(int fd, bool blocking);