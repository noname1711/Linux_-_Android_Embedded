.PHONY: all clean server client

CC := gcc
CFLAGS := -Wall -g

all: server client

server:
	$(CC) $(CFLAGS) server.c -o server
	@MES="$(MES)" ./server

client:
	$(CC) $(CFLAGS) client.c -o client
	@MES="$(MES)" ./client

clean:
	rm -f server client fifo_cs fifo_sc
