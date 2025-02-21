#include <stdio.h>

#include "server.h"

/**
 * @brief Creates a server and returns it
 *
 * @param host Hostname of the server
 * @param port Port of the server
 * @return Server_t* Pointer to new server or NULL if error
 */
Server_t* create_server(char host[], int port) {
  // initialize server
  Server_t* server = malloc(sizeof(Server_t));
  if (server == NULL) {
    printf("[ERROR] Could not malloc server!\n");
    return NULL;
  }

  // set host and port
  strncpy(server->host, host, INET_ADDRSTRLEN);
  server->port = port;

  // create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    printf("[ERROR] Could not create socket!\n");
    printf("errno: %d\n", errno);

    free(server);
    return NULL;
  }

  // set socket to be reusable
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    printf("[ERROR] Could not set socket options!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    free(server);
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
    free(server);
    return NULL;
  }

  // set server socket
  server->socket = server_socket;

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

    close_server(server);
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
      server->clients[i] = create_client(host, client_socket);
      break;
    }
  }

  return 0;
}

/**
 * @brief Sends a message to the client
 *
 * @param server Server_t struct
 * @param client Client_t struct
 * @param buff Message buffer
 * @param buff_len Length of the message
 * @return int 0 if successful, -1 if error
 */
int send_message(Server_t* server, Client_t* client, const char buff[], size_t buff_len) {
  // send message
  return send_client(client, buff, buff_len);
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