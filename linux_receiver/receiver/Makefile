CC = gcc
CFLAGS = -Wall -Wextra -lpthread

receiver: receiver.o misc.o
	$(CC) $(CFLAGS) -o receiver receiver.o misc.o

receiver.o: receiver.c receiver.h
	$(CC) $(CFLAGS) -c receiver.c

misc.o: misc.c
	$(CC) $(CFLAGS) -c misc.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c
