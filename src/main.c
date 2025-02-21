#include <stdio.h>

#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // check arguments
  if (argc < 2) {
    printf("[ERROR] Usage: %s <port>\n", argv[0]);
    return 1;
  }

  // create socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    printf("[ERROR] Could not create socket!\n");
    printf("errno: %d\n", errno);
    return 1;
  }

  printf("[INFO] Created socket!\n");

  // set socket to be reusable
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    printf("[ERROR] Could not set socket options!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    return 1;
  }

  // initialize server address
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));

  // set server address
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  // bind socket to server address
  if (bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("[ERROR] Could not bind socket!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    return 1;
  }

  printf("[INFO] Bound socket!\n");

  // listen for connections
  if (listen(server_socket, 5) == -1) {
    printf("[ERROR] Could not listen on socket!\n");
    printf("errno: %d\n", errno);

    close(server_socket);
    return 1;
  }

  printf("[INFO] Listening on 0.0.0.0:%d\n", atoi(argv[1]));

  // accept connections
  while (1) {
    // initialize client address
    struct sockaddr_in client_addr;
    int sz_client_addr = sizeof(client_addr);

    // accept connection
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &sz_client_addr);
    if (client_socket == -1) {
      printf("\n[ERROR] Could not accept client!\n");
      printf("errno: %d\n", errno);

      close(server_socket);
      return 1;
    }

    printf("\n[INFO] Accepted client!\n");
    printf("[INFO] Client: %s\n", inet_ntoa(client_addr.sin_addr));

    // initialize message buffer
    char buff[] = "hello client\n";

    // send data to client
    int sent_len = send(client_socket, buff, strlen(buff), 0);
    if (sent_len == -1) {
      printf("\n[ERROR] Could not send data to client!\n");
      printf("errno: %d\n", errno);

      close(client_socket);
      continue;
    }

    printf("[INFO] Sent data to client!\n");

    // close client socket
    close(client_socket);
  }

  // close server socket
  close(server_socket);
  return 0;
}