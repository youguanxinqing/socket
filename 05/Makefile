CC = gcc
CFLAGS = -std=c99

all : s c

s : server.o
	$(CC) server.o -o s $(CFLAGS)

c : client.o
	$(CC) client.o -o c $(CFLAGS)

server.o : server.c
	$(CC) -c server.c $(CFLAGS)

client.o : client.c
	$(CC) -c client.c $(CFLAGS)

.PHONY : clean

clean: 
	-rm server.o client.o s c
