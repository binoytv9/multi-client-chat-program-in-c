#include"myhdr.h"
#include<sys/select.h>

void *chat(int sockfd, struct addrinfo *p);

int main()
{
	int sockfd;
	struct addrinfo hints;
	struct addrinfo *res, *p;

	initialise(&hints);

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

	while("TRUE")
		if(chat(sockfd, p) == NULL)
			break;

	close(sockfd);
	freeaddrinfo(res);

	return 0;
}

void *chat(int sockfd, struct addrinfo *p)
{

	char *bufp;
	size_t size;
	int numBytes;
	fd_set readfds;
	char buf[MAXLEN];

	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	FD_SET(sockfd, &readfds);

	if(select(sockfd+1, &readfds, NULL, NULL, NULL) == -1)
		errExit("select");

	if(FD_ISSET(0, &readfds)){
		bufp = fgets(buf, sizeof(buf), stdin);
		if(bufp == NULL)
			size = 0;
		else{
			size = strlen(buf);
			buf[size-1] = '\0';
		}

		if(sendto(sockfd, buf, size, 0, p->ai_addr, p->ai_addrlen) == -1)
			errExit("sendto");

		if(bufp == NULL)
			return NULL;
	}

	if(FD_ISSET(sockfd, &readfds)){
		if((numBytes = recvfrom(sockfd, buf, sizeof buf - 1, 0, NULL, NULL)) == -1)
			errExit("recvfrom");

		buf[numBytes] = '\0';
		printf("%s\n", buf);
	}

	return p;
 }
