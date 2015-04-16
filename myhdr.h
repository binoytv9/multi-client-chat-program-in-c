#ifndef MYHDR_H
#define MYHDR_H

#include<stdio.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXLEN 100
#define MSGLEN 1000
#define SERVERPORT "8000"

void errExit(char *);
void initialise(struct addrinfo *);

#endif
