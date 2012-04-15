all: test

CC = gcc

CFLAGS = -g -Wall


test: test_server test_client
test_server: test_server.c
	$(CC) -o test_server test_server.c
test_client: test_client.c
	$(CC) -o test_client test_client.c


test_single: test.c
	$(CC) -o test test.c


clean:
	rm -f *.o
