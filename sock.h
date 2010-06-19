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
#define MAXBUF 100
#define MAX 100
#define MSGBUF 100

#define nick "fagbot"
#define channel "##fagbot"

int handler(char *, char *, char *);
