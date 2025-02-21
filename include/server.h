#ifndef SERVER_H
#define SERVER_H

#include "net.h"
#include "client.h"

// server struct
struct server {
  char host[INET_ADDRSTRLEN];
  int port;

  int socket;
  struct client* clients[5];
};

/* creates a server and returns it
 * if there was an error it returns NULL */
struct server* create_server(char host[], int port);

/* listens for connections on the server
 * if successful returns 0, otherwise returns -1 */
int listen_server(struct server* s);

/* accepts a connection on the server and appends to clients
 * if successful returns 0, otherwise returns -1 */
int accept_client(struct server* s);

/* sends a message to the client
 * if successful returns 0, otherwise returns -1 */
int send_message(struct server* s, struct client* c, char buff[]);

// closes a client connection
void close_connection(struct server* s, struct client* c);

// closes the server
void close_server(struct server* s);

#endif