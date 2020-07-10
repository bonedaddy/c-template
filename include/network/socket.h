/*! @file socket.h
  * @author Bonedaddy
  * @brief definitions, and helper functions related to socket usage in general
*/

#pragma once

/*! @def _POSIX_C_SOURCE 201112L
  * @brief fixes GCC not compiling correctly
  * currently we are handling this via our cmake build system (see CMakeLists.txt)
  * however you may want to define the macro in source yourself
  * @note https://stackoverflow.com/questions/39409846/why-does-gcc-not-complain-about-htons-but-complains-about-getaddrinfo-when-c/39410095#39410095
*/

#include <pthread.h>
#include "../utils/logger.h"
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


/*! @brief used to lock writes for _do_exit
  * @warning should not be called libraries using socket.c
*/
pthread_mutex_t _signal_mutex;
/*! @brief used to indicate when threads need to cleanup
  * @warning should not be called libraries using socket.c
  * polled at the beginning of every async_listen_func loop and async_handle_conn_func
  * when set to true causes active pthreads to exit after successfully cleaning up
*/
bool _do_exit;



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

/*! @typedef socket_client
  * @struct socket_client
  * a generic tcp socket client
*/
typedef struct socket_client {
    int socket_number;
} socket_client;

/*! @brief returns a new socket client connected to `addr:port`
*/
socket_client *new_socket_client(thread_logger *thl, addr_info hints, char *addr, char *port);

/*! @brief  gets an available socket attached to bind_address
  * @return Success: file descriptor socket number greater than 0
  * @return Failure: -1
  * initializers a socket attached to bind_address with sock_opts, and binds the address
*/
int get_new_socket(thread_logger *thl, addr_info *bind_address, SOCKET_OPTS sock_opts[], int num_opts);

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


