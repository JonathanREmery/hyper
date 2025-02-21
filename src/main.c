#include <stdio.h>

#include "server.h"
#include "request.h"

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
  ServerResult_t server_result;
  ServerCleanup_t server_cleanup;

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
  server = create_server(host, port, &server_result, &server_cleanup);
  if (server_result != SERVER_SUCCESS) {
    printf("[ERROR] Could not create server!\n");

    if (server_cleanup.socket_created) {
      close(server_cleanup.socket);
    }

    if (server_cleanup.server_allocated) {
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

    // loop over clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (server->clients[i] == NULL) {
        continue;
      }

      // initialize request buffer
      char raw_request[1024];

      // recieve request
      if (recieve_request(server, server->clients[i], raw_request, sizeof(raw_request)) == -1) {
        close_connection(server, server->clients[i]);
        continue;
      }

      // initialize request
      Request_t* request = NULL;
      RequestResult_t request_result;
      RequestCleanup_t request_cleanup = {0};

      // parse request
      request = parse_request(raw_request, &request_result, &request_cleanup);
      if (request_result != REQUEST_SUCCESS) {
        printf("[ERROR] Could not parse request!\n");

        // free request if needed
        if (request_cleanup.request_allocated) {
          free(request);
        }

        // close connection
        close_connection(server, server->clients[i]);
        continue;
      }

      // initialize response
      const char response[] = "<h1>Hello HTTP</h1>";

      // send response
      if (send_response(server, server->clients[i], response, strlen(response)) == -1) {
        close_connection(server, server->clients[i]);
        continue;
      }

      // close connection
      close_connection(server, server->clients[i]);
    }
  }

  // close server
  close_server(server);
  return 0;
}