/**
 * @file client.h
 * @brief Client connection management for hyper project
 */

#ifndef HYPER_CLIENT_H
#define HYPER_CLIENT_H

#include <stddef.h>
#include <stdint.h>
#include "net.h"

/**
 * @brief Client connection struct
 */
typedef struct {
  char host[INET_ADDRSTRLEN]; /**< Hostname of the client */
  int socket;                 /**< Socket of the client   */
} Client_t;

/**
 * @brief Result of client operations
 */
typedef enum {
  CLIENT_SUCCESS = 0,
  CLIENT_ERR_MALLOC = -1,
  CLIENT_ERR_SEND = -2
} ClientResult_t;

/**
 * @brief Client cleanup struct
 */
typedef struct {
  int client_allocated;
} ClientCleanup_t;

/**
 * @brief Creates a client connection
 *
 * @param host Hostname of the client
 * @param client_socket Socket of the client
 * @param result Result of the operation
 * @param cleanup Client cleanup struct
 * @return Client_t* Pointer to new client or NULL if error
 */
Client_t* create_client(char host[], int client_socket, ClientResult_t* result, ClientCleanup_t* cleanup);

/**
 * @brief Sends a message to the client
 *
 * @param client Client connection struct
 * @param buff Buffer of the message
 * @param buff_len Length of the message
 * @param result Result of the operation
 */
void send_client(Client_t* client, const char buff[], size_t buff_len, ClientResult_t* result);

/**
 * @brief Closes a client connection
 *
 * @param client Client connection struct
 */
void close_client(Client_t* client);

#endif