/**
 * @file server.h
 * @brief Server implementation for hyper project
 */

#ifndef HYPER_SERVER_H
#define HYPER_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#include "net.h"
#include "client.h"
#include "request.h"

/** Maximum number of concurrent clients */
#define MAX_CLIENTS 5

/**
 * @brief Server struct
 */
typedef struct {
  char host[INET_ADDRSTRLEN];          /**< Hostname of the server */
  int port;                            /**< Port of the server     */
  int socket;                          /**< Socket of the server   */
} server_t;

/**
 * @brief Result of server operations
 */
typedef enum {
  SERVER_SUCCESS = 0,
  SERVER_ERR_MALLOC = -1,
  SERVER_ERR_SOCKET = -2,
  SERVER_ERR_SETSOCKOPT = -3,
  SERVER_ERR_BIND = -4,
  SERVER_ERR_LISTEN = -5,
  SERVER_ERR_ACCEPT = -6,
} server_result_t;

/**
 * @brief Server cleanup struct
 */
typedef struct {
  int server_allocated;
  int socket_created;
  int socket;
} server_cleanup_t;

/**
 * @brief Creates a server and returns it
 *
 * @param host Hostname of the server
 * @param port Port of the server
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return server_t* Pointer to new server or NULL if error
 */
server_t* create_server(char host[], int port, server_result_t* result, server_cleanup_t* cleanup);

/**
 * @brief Listens for connections on the server
 *
 * @param server server_t struct
 * @return int 0 if successful, -1 if error
 */
int listen_server(server_t* server);

/**
 * @brief Accepts a connection on the server and appends to clients
 *
 * @param server server_t struct
 * @return client_t* Pointer to new client or NULL if error
 */
 client_t* accept_client(server_t* server);

/**
 * @brief Creates handler threads
 * 
 * @param server server_t struct
 * @param client client_t struct
 * @return int 0 if successful, -1 if error
 */
int handle_client(server_t* server, client_t* client);

/**
 * @brief Handles client requests and responds accordingly in a thread
 *
 * @param argp client_t struct
 * @return void* NULL
 */
void* handle_client_thread(void* argp);

/**
 * @brief Closes the server
 *
 * @param server server_t struct
 */
void close_server(server_t* server);

#endif