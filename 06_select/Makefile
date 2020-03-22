CC = gcc
CFLAGS = -std=c99

OBJS = client.o server.o

all: s c

c : client.o
	$(CC) client.o -o c $(CFLAGS)

s : server.o
	$(CC) server.o -o s $(CFLAGS)

client.o : client.c
	$(CC) -c client.c $(CFLAGS)

server.o : server.c
	$(CC) -c server.c $(CFLAGS)

.PHONY : clean

clean :
	-rm $(OBJS) s c
