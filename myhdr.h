#ifndef MYHDR_H
#define MYHDR_H

#include<stdio.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAXLEN 100
#define SERVERPORT "8000"

void errExit(char *);

#endif
