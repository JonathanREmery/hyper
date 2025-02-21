/**
 * @file server.h
 * @brief Server implementation for hyper project
 */

#ifndef HYPER_SERVER_H
#define HYPER_SERVER_H

#include <stddef.h>
#include <stdint.h>

#include "net.h"
#include "client.h"

/** Maximum number of concurrent clients */
#define MAX_CLIENTS 5

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
} ServerResult_t;

/**
 * @brief Server cleanup struct
 */
typedef struct {
  int server_allocated;
  int socket_created;
  int socket;
} ServerCleanup_t;

/**
 * @brief Server struct
 */
typedef struct {
  char host[INET_ADDRSTRLEN];          /**< Hostname of the server */
  int port;                            /**< Port of the server     */
  int socket;                          /**< Socket of the server   */
  Client_t* clients[MAX_CLIENTS]; /**< Clients of the server  */
} Server_t;

/**
 * @brief Creates a server and returns it
 *
 * @param host Hostname of the server
 * @param port Port of the server
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return Server_t* Pointer to new server or NULL if error
 */
Server_t* create_server(char host[], int port, ServerResult_t* result, ServerCleanup_t* cleanup);

/**
 * @brief Listens for connections on the server
 *
 * @param server Server_t struct
 * @return int 0 if successful, -1 if error
 */
int listen_server(Server_t* server);

/**
 * @brief Accepts a connection on the server and appends to clients
 *
 * @param server Server_t struct
 * @return int 0 if successful, -1 if error
 */
int accept_client(Server_t* server);

/**
 * @brief Sends a message to the client
 *
 * @param server Server_t struct
 * @param client Client_t struct
 * @param buff Message buffer
 * @param buff_len Length of the message
 * @return int 0 if successful, -1 if error
 */
int send_message(Server_t* server, Client_t* client, const char buff[], size_t buff_len);

/**
 * @brief Closes a client connection
 *
 * @param server Server_t struct
 * @param client Client_t struct
 */
void close_connection(Server_t* server, Client_t* client);

/**
 * @brief Closes the server
 *
 * @param server Server_t struct
 */
void close_server(Server_t* server);

#endif