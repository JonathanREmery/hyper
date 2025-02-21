#include <stdio.h>

#include "server.h"

/**
 * @brief Creates a server and returns it
 *
 * @param host Hostname of the server
 * @param port Port of the server
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return Server_t* Pointer to new server or NULL if error
 */
Server_t* create_server(char host[], int port, ServerResult_t* result, ServerCleanup_t* cleanup) {
  // initialize result
  *result = SERVER_SUCCESS;

  // initialize cleanup
  cleanup->server_allocated = 0;
  cleanup->socket_created = 0;
  cleanup->socket = 0;

  // initialize server
  Server_t* server = malloc(sizeof(Server_t));
  if (server == NULL) {
    *result = SERVER_ERR_MALLOC;
    return NULL;
  }
  cleanup->server_allocated = 1;

  // set host and port
  strncpy(server->host, host, INET_ADDRSTRLEN);
  server->port = port;

  // create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    *result = SERVER_ERR_SOCKET;
    return NULL;
  }

  // set server socket
  server->socket = server_socket;

  // set cleanup socket created and socket
  cleanup->socket_created = 1;
  cleanup->socket = server_socket;

  // set socket to be reusable
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    *result = SERVER_ERR_SETSOCKOPT;
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
    *result = SERVER_ERR_BIND;
    return NULL;
  }

  // initialize client array
  for (int i = 0; i < MAX_CLIENTS; i++) {
    server->clients[i] = NULL;
  }

  // return server
  return server;
}

/**
 * @brief Listens for connections on the server
 *
 * @param server Server_t struct
 * @return int 0 if successful, -1 if error
 */
int listen_server(Server_t* server) {
  // listen for connections
  if (listen(server->socket, MAX_CLIENTS) == -1) {
    printf("[ERROR] Could not listen on socket!\n");
    printf("errno: %d\n", errno);

    close_server(server);
    return -1;
  }

  return 0;
}

/**
 * @brief Accepts a connection on the server and appends to clients
 *
 * @param server Server_t struct
 * @return int 0 if successful, -1 if error
 */
int accept_client(Server_t* server) {
  // initialize client address
  struct sockaddr_in client_addr;
  socklen_t sz_client_addr = sizeof(client_addr);

  // accept connection
  int client_socket = accept(server->socket, (struct sockaddr*)&client_addr, &sz_client_addr);
  if (client_socket == -1) {
    printf("[ERROR] Could not accept connection!\n");
    printf("errno: %d\n", errno);

    return -1;
  }

  printf("\n[INFO] Accepted client!\n");

  // get client host
  char host[INET_ADDRSTRLEN];
  strncpy(host, inet_ntoa(client_addr.sin_addr), INET_ADDRSTRLEN);

  printf("[INFO] Client: %s\n", host);

  // add client to server
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (server->clients[i] == NULL) {
      // initialize cleanup
      ClientResult_t result;
      ClientCleanup_t cleanup;

      // create client
      server->clients[i] = create_client(host, client_socket, &result, &cleanup);

      // check result
      if (result != CLIENT_SUCCESS) {
        // cleanup if needed
        if (cleanup.client_allocated) {
          // free client
          close_connection(server, server->clients[i]);
        }
      }

      break;
    }
  }

  return 0;
}

/**
 * @brief Recieves a request from the client
 *
 * @param server Server_t struct
 * @param client Client_t struct
 * @param buff Request buffer
 * @param buff_len Length of the request
 * @return int 0 if successful, -1 if error
 */
int recieve_request(Server_t* server, Client_t* client, char buff[], size_t buff_len) {
  // initialize client result
  ClientResult_t result;

  // recieve request
  recv_client(client, buff, buff_len, &result);

  // check result
  if (result != CLIENT_SUCCESS) {
    return -1;
  }

  return 0;
}

/**
 * @brief Sends a response to the client
 *
 * @param server Server_t struct
 * @param client Client_t struct
 * @param buff Response buffer
 * @param buff_len Length of the response
 * @return int 0 if successful, -1 if error
 */
int send_response(Server_t* server, Client_t* client, const char buff[], size_t buff_len) {
  // initialize client result
  ClientResult_t result;

  // send response
  send_client(client, buff, buff_len, &result);

  // check result
  if (result != CLIENT_SUCCESS) {
    return -1;
  }

  return 0;
}

/**
 * @brief Closes a client connection
 *
 * @param server Server_t struct
 * @param client Client_t struct
 */
void close_connection(Server_t* server, Client_t* client) {
  // close client
  close_client(client);

  // find client pointer array an NULL it out
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (server->clients[i] == client) {
      server->clients[i] = NULL;
      break;
    }
  }
}

/**
 * @brief Closes the server
 *
 * @param server Server_t struct
 */
void close_server(Server_t* server) {
  // close socket
  close(server->socket);

  // free server
  free(server);
}