#include <stdio.h>

#include "server.h"

/**
 * @brief Creates a server and returns it
 *
 * @param host Hostname of the server
 * @param port Port of the server
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return server_t* Pointer to new server or NULL if error
 */
server_t* create_server(char host[], int port, server_result_t* result, server_cleanup_t* cleanup) {
  // initialize result
  *result = SERVER_SUCCESS;

  // initialize cleanup
  cleanup->server_allocated = 0;
  cleanup->socket_created = 0;
  cleanup->socket = 0;

  // initialize server
  server_t* server = malloc(sizeof(server_t));
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

  // return server
  return server;
}

/**
 * @brief Listens for connections on the server
 *
 * @param server server_t struct
 * @return int 0 if successful, -1 if error
 */
int listen_server(server_t* server) {
  // listen for connections
  if (listen(server->socket, MAX_CLIENTS) == -1) {
    log_message(LOG_ERROR, "Could not listen on socket!\n");

    close_server(server);
    return -1;
  }

  return 0;
}

/**
 * @brief Accepts a connection on the server and appends to clients
 *
 * @param server server_t struct
 * @return client_t* Pointer to new client or NULL if error
 */
client_t* accept_client(server_t* server) {
  // initialize client address
  struct sockaddr_in client_addr;
  socklen_t sz_client_addr = sizeof(client_addr);

  // accept connection
  int client_socket = accept(server->socket, (struct sockaddr*)&client_addr, &sz_client_addr);
  if (client_socket == -1) {
    log_message(LOG_ERROR, "Could not accept connection!\n");

    return NULL;
  }

  // get client host
  char host[INET_ADDRSTRLEN];
  strncpy(host, inet_ntoa(client_addr.sin_addr), INET_ADDRSTRLEN);

  // initialize client variables
  client_t* client;
  client_result_t result;
  client_cleanup_t cleanup;

  // create client
  client = create_client(host, client_socket, &result, &cleanup);

  // check result
  if (result != CLIENT_SUCCESS) {
    // cleanup if needed
    if (cleanup.client_allocated) {
      close_client(client);
    }

    return NULL;
  }

  return client;
}

/**
 * @brief Creates handler threads
 * 
 * @param server server_t struct
 * @param client client_t struct
 * @return int 0 if successful, -1 if error
 */
int handle_client(server_t* server, client_t* client) {
  // create thread
  pthread_t handler_thread;
  if (pthread_create(&handler_thread, NULL, handle_client_thread, client) != 0) {
    log_message(LOG_ERROR, "Failed to create handler thread!\n");
    return -1;
  }

  // detach thread
  pthread_detach(handler_thread);

  return 0;
}

/**
 * @brief Handles client requests and responds accordingly in a thread
 *
 * @param argp client_t struct
 * @return void* NULL
 */
void* handle_client_thread(void* argp) {
  // initialize client
  client_t* client = (client_t*)argp;
  client_result_t client_result;
  client_cleanup_t client_cleanup = {0};

  while (1) {
    // initialize request buffer
    char raw_request[1024];

    // recieve request
    recv_client(client, raw_request, sizeof(raw_request), &client_result);
    if (client_result != CLIENT_SUCCESS) {
      break;
    }

    // initialize request variables
    request_t* request;
    request_result_t request_result;
    request_cleanup_t request_cleanup = {0};

    // parse request
    request = parse_request(raw_request, &request_result, &request_cleanup);
    if (request_result != REQUEST_SUCCESS) {
      if (request_cleanup.request_allocated) {
        free(request_cleanup.request);
      }

      break;
    }

    // handle request
    handle_request(client, request);
    break;
  }

  // close client
  close_client(client);
  return NULL;
}

/**
 * @brief Handles a request from a client
 *
 * @param client client_t struct
 * @param request request_t struct
 * @return int 0 if successful, -1 if error
 */
int handle_request(client_t* client, request_t* request) {
  // initialize result
  client_result_t result;

  // log request
  char message[512];
  sprintf(message, "Serving %s to %s\n", request->file_name, client->host);
  log_message(LOG_INFO, message);

  // initialize response
  char response[MAX_RESPONSE_LENGTH];
  char file_content[MAX_FILE_LENGTH];

  // read file into response
  int fd = open(request->file_name, O_RDONLY);
  read(fd, file_content, sizeof(file_content));
  close(fd);

  // craft response
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%s", strlen(file_content), file_content);

  // send response
  send_client(client, response, sizeof(response), &result);

  // check result
  if (result != CLIENT_SUCCESS) {
    return -1;
  }

  return 0;
}

/**
 * @brief Closes the server
 *
 * @param server server_t struct
 */
void close_server(server_t* server) {
  // close socket
  close(server->socket);

  // free server
  free(server);
}