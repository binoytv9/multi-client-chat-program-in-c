#include"myhdr.h"

void errExit(char *msg)
{
	perror(msg);
	exit(1);
}

void initialise(struct addrinfo *hints)
{
	memset(hints, 0, sizeof(struct addrinfo));

	hints->ai_protocol = 0;
	hints->ai_flags = AI_PASSIVE;
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_DGRAM;
}
