CC = gcc
CFLAGS = -std=c99

OBJS = common.o server.o client.o

S_REQ = server.o common.o
C_REQ = client.o common.o

all : s c

s : $(S_REQ)
	$(CC) $(S_REQ) -o s $(CFLAGS)

c : $(C_REQ)
	$(CC) $(C_REQ) -o c $(CFLAGS)


common.o : common.h
	$(CC) -c common.c $(CFLAGS)

client.o : client.c
	$(CC) -c client.c $(CFLAGS)

server.o : server.c common.h
	$(CC) -c server.c $(CFLAGS)



.PHONY : clean
clean:
	-rm $(OBJS) s c
