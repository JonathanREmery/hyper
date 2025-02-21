#include "logger.h"
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
  server_t* server;
  server_result_t server_result;
  server_cleanup_t server_cleanup;
  char message[512];

  // print usage if arguments are not enough
  if (argc < 3) {
    sprintf(message, "Usage: %s <host> <port>\n", argv[0]);
    log_message(LOG_ERROR, message);

    return -1;
  }

  // get host
  if (strlen(argv[1]) >= INET_ADDRSTRLEN) {
    log_message(LOG_ERROR, "Host is too long!\n");
    return -1;
  }
  strncpy(host, argv[1], INET_ADDRSTRLEN);

  // get port
  port = atoi(argv[2]);
  if (port <= 0 || port > 65535) {
    log_message(LOG_ERROR, "Invalid port number!\n");
    return -1;
  }

  // create server
  server = create_server(host, port, &server_result, &server_cleanup);
  if (server_result != SERVER_SUCCESS) {
    log_message(LOG_ERROR, "Could not create server!\n");

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

  sprintf(message, "Listening on %s:%d\n", host, port);
  log_message(LOG_INFO, message);

  // accept connections
  while (1) {
    // accept client
    client_t* client = accept_client(server);
    if (client == NULL) {
      continue;
    }

    // handle client
    handle_client(server, client);
  }

  // close server
  close_server(server);
  return 0;
}