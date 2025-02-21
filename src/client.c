#include <stdio.h>

#include "client.h"

/**
 * @brief Creates a client connection
 *
 * @param host Hostname of the client
 * @param client_socket Socket of the client
 * @return Client_t* Pointer to new client or NULL if error
 */
Client_t* create_client(char host[], int client_socket) {
  // initialize client
  Client_t* c = malloc(sizeof(Client_t));
  if (c == NULL) {
    printf("[ERROR] Could not malloc client!\n");
    return NULL;
  }

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
 * @return int 0 if successful, -1 if error
 */
int send_client(Client_t* client, const char buff[], size_t buff_len) {
  // send message
  if (send(client->socket, buff, buff_len, 0) == -1) {
    printf("[ERROR] Could not send message!\n");
    printf("errno: %d\n", errno);

    return -1;
  }

  return 0;
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