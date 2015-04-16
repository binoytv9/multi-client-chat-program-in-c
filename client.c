#include"myhdr.h"

int main()
{
	int sockfd;
	char buf[MAXLEN];
	struct addrinfo *res, *p;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = 0;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL, SERVERPORT, &hints, &res) != 0)
		errExit("getaddrinfo");

	for(p = res; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		break;
	}

	if(p == NULL){
		fprintf(stderr, "unable to bind to address\n");
		exit(1);
	}

	while(fgets(buf, sizeof(buf), stdin) != NULL){
		size_t size = strlen(buf);
		buf[size-1] = '\0';
		if(sendto(sockfd, buf, size, 0, p->ai_addr, p->ai_addrlen) == -1)
			errExit("sendto");
	}

	close(sockfd);
	
	freeaddrinfo(res);

	return 0;
}
