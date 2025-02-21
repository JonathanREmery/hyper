#include <stdio.h>

#include "server.h"

/**
 * @brief Main function
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int 0 if successful, -1 if error
 */
int main(int argc, char *argv[]) {
  char host[INET_ADDRSTRLEN];
  int port;
  Server_t* server;
  ServerResult_t result;
  ServerCleanup_t cleanup;

  // print usage if arguments are not enough
  if (argc < 3) {
    printf("[ERROR] Usage: %s <host> <port>\n", argv[0]);
    return -1;
  }

  // get host
  if (strlen(argv[1]) >= INET_ADDRSTRLEN) {
    printf("[ERROR] Host is too long!\n");
    return -1;
  }
  strncpy(host, argv[1], INET_ADDRSTRLEN);

  // get port
  port = atoi(argv[2]);
  if (port <= 0 || port > 65535) {
    printf("[ERROR] Invalid port number!\n");
    return -1;
  }

  // create server
  server = create_server(host, port, &result, &cleanup);
  if (result != SERVER_SUCCESS) {
    printf("[ERROR] Could not create server!\n");

    if (cleanup.socket_created) {
      close(cleanup.socket);
    }

    if (cleanup.server_allocated) {
      free(server);
    }

    return -1;
  }

  // listen for connections
  if (listen_server(server) == -1) {
    close_server(server);
    return -1;
  }

  printf("[INFO] Listening on %s:%d\n", host, port);

  // accept connections
  while (1) {
    // accept client
    if (accept_client(server) == -1) {
      close_server(server);
      return -1;
    }

    // initialize message buffer
    char buff[] = "hello client\n";

    // send data to clients
    for (int i = 0; i < 5; i++) {
      if (server->clients[i] == NULL) {
        continue;
      }

      // send data to client
      if (send_message(server, server->clients[i], buff, sizeof(buff)) == -1) {
        close_connection(server, server->clients[i]);
        continue;
      }

      printf("[INFO] Sent data to client!\n");

      // close client connection
      close_connection(server, server->clients[i]);
    }
  }

  // close server
  close_server(server);
  return 0;
}