#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

// server struct
struct server {
  char host[INET_ADDRSTRLEN];
  int port;

  int socket;
  int clients[5];
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

// closes a client connection
void close_client(struct server* s, int client_socket);

// closes the server
void close_server(struct server* s);

#endif