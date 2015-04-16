CC = cc
CFLAGS = -Wall -g

default: server client

server: server.o myfun.o
	$(CC) $(CFLAGS) server.o myfun.o -o server

server.o: server.c myhdr.h
	$(CC) $(CFLAGS) -c server.c

client: client.o myfun.o
	$(CC) $(CFLAGS) client.o myfun.o -o client

client.o: client.c myhdr.h
	$(CC) $(CFLAGS) -c client.c

myfun.o: myfun.c myhdr.h
	$(CC) $(CFLAGS) -c myfun.c

clean:
	$(RM) *.out *.o  *~ server client
