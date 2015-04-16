#include"myhdr.h"

struct client{
	char ip[INET6_ADDRSTRLEN];
	struct sockaddr src_addr;
	int src_addrlen;
	struct client *next;
};

struct client *newNode(struct sockaddr *src_addr, int src_addrlen, char *ip);
void sendAll(char *msg, struct client **headRef, int sockfd, struct sockaddr *src_addr, int src_addrlen, char *ip);

int main()
{
	int sockfd;
	int numBytes;
	char buf[MAXLEN];
	socklen_t src_addrlen;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct sockaddr src_addr;
	char dst_addr[INET6_ADDRSTRLEN];
	char msg[MSGLEN];
	struct client *clientList;

	initialise(&hints);

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

	if(inet_ntop(p->ai_family, &src_addr, dst_addr, sizeof dst_addr) == NULL)
		errExit("inet_ntop");
	printf("\tserver@%s\n\n", dst_addr);

	freeaddrinfo(res);

	clientList = NULL;
	src_addrlen = sizeof(struct sockaddr);
	while("TRUE"){
		numBytes = recvfrom(sockfd, buf, sizeof buf - 1, 0, &src_addr, &src_addrlen);
		if(numBytes == -1)
			errExit("recvfrom");

		if(inet_ntop(src_addr.sa_family, &src_addr, dst_addr, sizeof dst_addr) == NULL)
			errExit("inet_ntop");

		if(numBytes == 0)
			snprintf(msg, sizeof msg, "%s closed connection!!!", dst_addr);
		else{
			buf[numBytes] = '\0';
			snprintf(msg, sizeof msg, "%s>> %s", dst_addr, buf);
		}
		printf("%s\n", msg);

		sendAll(msg, &clientList, sockfd, &src_addr, src_addrlen, dst_addr);

		src_addrlen = sizeof(struct sockaddr);
	}

	close(sockfd);
	return 0;
}


void sendAll(char *msg, struct client **headRef, int sockfd, struct sockaddr *src_addr, int src_addrlen, char *ip)
{
	int found = 0;
	struct client *new, *current;

	current = *headRef;
	while(current != NULL){
		if(strcmp(current->ip, ip) == 0)
			found = 1;
		else{
			if(sendto(sockfd, msg, strlen(msg), 0, &(current->src_addr), current->src_addrlen) == -1)
				errExit("sendto");
		}

		current = current->next;
	}

	if(!found){
		new = newNode(src_addr, src_addrlen, ip);
		new->next = *headRef;
		*headRef = new;
	}
}

struct client *newNode(struct sockaddr *src_addr, int src_addrlen, char *ip)
{
	struct client *new = malloc(sizeof(struct client));

	strcpy(new->ip, ip);
	memmove(&(new->src_addr), src_addr, sizeof(struct sockaddr));
	new->src_addrlen = src_addrlen;
	new->next = NULL;

	return new;
}
