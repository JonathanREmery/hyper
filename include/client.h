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
 * @brief Creates a client connection
 *
 * @param host Hostname of the client
 * @param client_socket Socket of the client
 * @return Client_t* Pointer to new client or NULL if error
 */
Client_t* create_client(char host[], int client_socket);

/**
 * @brief Sends a message to the client
 *
 * @param client Client connection struct
 * @param buff Buffer of the message
 * @param buff_len Length of the message
 * @return int 0 if successful, -1 if error
 */
int send_client(Client_t* client, const char buff[], size_t buff_len);

/**
 * @brief Closes a client connection
 *
 * @param client Client connection struct
 */
void close_client(Client_t* client);

#endif