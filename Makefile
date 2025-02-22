CC=gcc
CFLAGS=-Wall -Iinclude -o bin/hyper

hyper: src/main.c src/server.c src/client.c src/request.c src/logger.c
	@mkdir -p bin
	$(CC) $(CFLAGS) src/main.c src/server.c src/client.c src/request.c src/logger.c

clean:
	@rm -rf bin