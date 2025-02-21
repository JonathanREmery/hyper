#include <stdio.h>

#include "client.h"

/* creates a client and returns it
 * if there was an error it returns NULL */
struct client* create_client(char host[], int client_socket) {
  // initialize client
  struct client* c = malloc(sizeof(struct client));
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

/* sends a message to the client
 * returns 0 if successful, otherwise returns -1 */
int send_client(struct client* c, char buff[]) {
  // send message
  if (send(c->socket, buff, strlen(buff), 0) == -1) {
    printf("[ERROR] Could not send message!\n");
    printf("errno: %d\n", errno);

    return -1;
  }

  return 0;
}

// closes a client connection
void close_client(struct client* c) {
  // close socket
  close(c->socket);

  // free client
  free(c);
}