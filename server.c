#include"myhdr.h"

int main()
{
	int sockfd;
	int numBytes;
	char buf[MAXLEN];
	socklen_t src_addrlen;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct sockaddr src_addr;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL, SERVERPORT, &hints, &res) != 0)
		errExit("getaddrinfo");

	for(p = res; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			continue;
		}

		break;
	}

	if(p == NULL){
		fprintf(stderr, "unable to bind to address\n");
		exit(1);
	}

	freeaddrinfo(res);

	src_addrlen = sizeof(struct sockaddr);
	while((numBytes = recvfrom(sockfd, buf, sizeof(buf)-1, 0, &src_addr, &src_addrlen)) != 0){
		if(numBytes == -1)
			errExit("recvfrom");

		buf[numBytes] = '\0';

		printf("%s\n", buf);
		src_addrlen = sizeof(struct sockaddr);
	}

	return 0;
}
