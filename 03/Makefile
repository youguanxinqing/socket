CC = gcc
CFLAGS = -std=c99


all : s c

s : server.c
	$(CC) server.c -o s $(CFLAGS)

c : client.c
	$(CC) client.c -o c $(CFLAGS)

.PHONY : clean

clean:
	-rm s c

