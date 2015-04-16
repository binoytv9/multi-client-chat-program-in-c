#include"myhdr.h"

void errExit(char *msg)
{
	perror(msg);
	exit(1);
}
