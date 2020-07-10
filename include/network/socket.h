/*! @file socket.h
  * @author Bonedaddy
  * @brief a generic multi-threaded tcp socket server
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

/*! @typedef socket_client
  * @struct socket_client
  * a generic tcp socket client
*/
typedef struct socket_client {
    int socket_number;
} socket_client;

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

/*! @enum SOCKET_OPTS
  * @brief used to configure new sockets
*/
typedef enum {
  /*! sets socket with SO_REUSEADDR */
  REUSEADDR,
  /*! sets socket to non-blocking mode */
  NOBLOCK,
  /*! sets socket to blocking mode */
  BLOCK,
} SOCKET_OPTS;

/*! @brief returns a new socket server bound to the port number and ready to accept connections
*/
socket_server *new_socket_server(addr_info hints, thread_logger *thl, int max_conns, char *port);

/*! @brief  gets an available socket attached to bind_address
  * @return Success: file descriptor socket number greater than 0
  * @return Failure: -1
  * initializers a socket attached to bind_address with sock_opts, and binds the address
*/
int get_new_socket(thread_logger *thl, addr_info *bind_address, SOCKET_OPTS sock_opts[], int num_opts);

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

/*! @brief prepares library for usage
  * @warning must be called before using the library
  * sets up internal mutex, and system signal handling for terminating the server
  * listes to SIGINT, SIGTERM, and SIGQUIT which will terminate the server
*/
void setup_signal_handling();

/*! @brief used to enable/disable blocking sockets
  * @return Failure: false
  * @return Success: true
  * @note see https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking/1549344#1549344
*/
bool set_socket_blocking_status(int fd, bool blocking);


/*! @brief callback function used to handle OS signals
  * shouldn't be called directly and instead used as the func in `signal(SIGTERM, handler_fn)`
*/
void signal_handler_fn(int signal_number);

/*! @brief returns the address the client is connecting from
*/
char  *get_name_info(sock_addr *client_address);

/*! @brief generates an addr_info struct with defaults
  * defaults is IPv4, TCP, and AI_PASSIVE flags
*/
addr_info default_hints();


/*! @brief returns a new socket client connected to `addr:port`
*/
socket_client *new_socket_client(thread_logger *thl, addr_info hints, char *addr, char *port);








