#include <stdio.h>

#include "client.h"

/**
 * @brief Creates a client connection
 *
 * @param host Hostname of the client
 * @param client_socket Socket of the client
 * @param result Result of the operation
 * @param cleanup Client cleanup struct
 * @return Client_t* Pointer to new client or NULL if error
 */
Client_t* create_client(char host[], int client_socket, ClientResult_t* result, ClientCleanup_t* cleanup) {
  // initialize result
  *result = CLIENT_SUCCESS;

  // initialize cleanup
  cleanup->client_allocated = 0;

  // initialize client
  Client_t* c = malloc(sizeof(Client_t));
  if (c == NULL) {
    *result = CLIENT_ERR_MALLOC;
    return NULL;
  }
  cleanup->client_allocated = 1;

  // set host and socket
  strncpy(c->host, host, INET_ADDRSTRLEN);
  c->socket = client_socket;

  // return client
  return c;
}

/**
 * @brief Sends a message to the client
 *
 * @param client Client connection struct
 * @param buff Buffer of the message
 * @param buff_len Length of the message
 * @param result Result of the operation
 */
void send_client(Client_t* client, const char buff[], size_t buff_len, ClientResult_t* result) {
  // initialize result
  *result = CLIENT_SUCCESS;

  // send message
  if (send(client->socket, buff, buff_len, 0) == -1) {
    *result = CLIENT_ERR_SEND;
  }
}

/**
 * @brief Closes a client connection
 *
 * @param client Client connection struct
 */
void close_client(Client_t* client) {
  // close socket
  close(client->socket);

  // free client
  free(client);
}