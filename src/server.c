#include <stdio.h>

#include "server.h"

/* creates a server and returns it
 * if there was an error it returns NULL */
struct server* create_server(char host[], int port) {
  // initialize server
  struct server* s = malloc(sizeof(struct server));
  if (s == NULL) {
    printf("[ERROR] Could not malloc server!\n");
    return NULL;
  }

  // set host and port
  strncpy(s->host, host, INET_ADDRSTRLEN);
  s->port = port;

  // create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    printf("[ERROR] Could not create socket!\n");
    printf("errno: %d\n", errno);

    free(s);
    return NULL;
  }

  // set socket to be reusable
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    printf("[ERROR] Could not set socket options!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    free(s);
    return NULL;
  }

  // initialize server address
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));

  // set server address
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(host);
  serv_addr.sin_port = htons(port);

  // bind socket to server address
  if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("[ERROR] Could not bind socket!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    free(s);
    return NULL;
  }

  // set server socket
  s->socket = server_socket;

  // initialize client array
  for (int i = 0; i < 5; i++) {
    s->clients[i] = NULL;
  }

  // return server
  return s;
}

/* listens for connections on the server
 * if successful returns 0, otherwise returns -1 */
int listen_server(struct server* s) {
  // listen for connections
  if (listen(s->socket, 5) == -1) {
    printf("[ERROR] Could not listen on socket!\n");
    printf("errno: %d\n", errno);

    close_server(s);
    return -1;
  }

  return 0;
}

/* accepts a connection on the server and appends to clients
 * if successful returns 0, otherwise returns -1 */
int accept_client(struct server* s) {
  // initialize client address
  struct sockaddr_in client_addr;
  socklen_t sz_client_addr = sizeof(client_addr);

  // accept connection
  int client_socket = accept(s->socket, (struct sockaddr*)&client_addr, &sz_client_addr);
  if (client_socket == -1) {
    printf("[ERROR] Could not accept connection!\n");
    printf("errno: %d\n", errno);

    close_server(s);
    return -1;
  }

  printf("\n[INFO] Accepted client!\n");

  // get client host
  char host[INET_ADDRSTRLEN];
  strncpy(host, inet_ntoa(client_addr.sin_addr), INET_ADDRSTRLEN);

  printf("[INFO] Client: %s\n", host);

  // add client to server
  for (int i = 0; i < 5; i++) {
    if (s->clients[i] == NULL) {
      s->clients[i] = create_client(host, client_socket);
      break;
    }
  }

  return 0;
}

/* sends a message to the client
 * if successful returns 0, otherwise returns -1 */
int send_message(struct server* s, struct client* c, char buff[]) {
  // send message
  return send_client(c, buff);
}

// closes a client connection
void close_connection(struct server* s, struct client* c) {
  // close client
  close_client(c);

  // find client pointer array an NULL it out
  for (int i = 0; i < 5; i++) {
    if (s->clients[i] == c) {
      s->clients[i] = NULL;
      break;
    }
  }
}

// closes the server
void close_server(struct server* s) {
  // close socket
  close(s->socket);

  // free server
  free(s);
}