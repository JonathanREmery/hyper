#include <stdio.h>
#include "server.h"

int main(int argc, char *argv[]) {
  // check arguments
  if (argc < 3) {
    printf("[ERROR] Usage: %s <host> <port>\n", argv[0]);
    return 1;
  }

  // create server
  char host[INET_ADDRSTRLEN];
  strncpy(host, argv[1], INET_ADDRSTRLEN);
  int port = atoi(argv[2]);
  struct server* s = create_server(host, port);

  // listen for connections
  if (listen_server(s) == -1) {
    close_server(s);
    return 1;
  }

  printf("[INFO] Listening on %s:%d\n", host, port);

  // accept connections
  while (1) {
    // accept client
    if (accept_client(s) == -1) {
      close_server(s);
      return 1;
    }

    // initialize message buffer
    char buff[] = "hello client\n";

    // send data to clients
    for (int i = 0; i < 5; i++) {
      if (s->clients[i] == -1) {
        continue;
      }

      // send data to client
      int sent_len = send(s->clients[i], buff, strlen(buff), 0);
      if (sent_len == -1) {
        printf("\n[ERROR] Could not send data to client!\n");
        printf("errno: %d\n", errno);

        close_client(s, s->clients[i]);
        continue;
      }

      printf("[INFO] Sent data to client!\n");

      // close client connection
      close_client(s, s->clients[i]);
    }
  }

  // close server
  close_server(s);
  return 0;
}