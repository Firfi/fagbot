#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>

/* Yes, these are all the same */
#define MAXBUF 8192
#define MAX 8192
#define MSGBUF 8192

int handler(char *, char *, char *);
