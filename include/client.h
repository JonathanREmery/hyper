#ifndef CLIENT_H
#define CLIENT_H

#include "net.h"

// client struct
struct client {
  char host[INET_ADDRSTRLEN];

  int socket;
};

/* creates a client and returns it
 * if there was an error it returns NULL */
struct client* create_client(char host[], int client_socket);

/* sends a message to the client
 * if successful returns 0, otherwise returns -1 */
int send_client(struct client* c, char buff[]);

// closes a client connection
void close_client(struct client* c);

#endif